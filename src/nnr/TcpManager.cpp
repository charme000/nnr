#include "TcpManager.h"

/*! \class TcpManager
    \brief 利用网络套接字实现通讯的管理器接口
    \version 1.0
    \date 2012.12.1-2012.1.30
    \pre 一般该类的函数不需要显式的调用,Base类会管理并且自动调用
  */

QT_BEGIN_NAMESPACE

/*! 网络套接字管理器构造函数
    \param bus 通讯总线
    \param attribute 管理器属性
    \param value 连接标识
    \param ip 供连接的IP地址字符串
    \param guidName 自身标识uuid

    \sa Manager(Bus& bus, QString& attribute, QString& value, QString &ip, QString& guidName):
  */
TcpManager::TcpManager(Bus& bus, QString& attribute, QString& value, QString &ip, QString& guidName)
    : Manager(bus, attribute, value, ip, guidName),m_socket(NULL),m_server(NULL)
{
    m_port = value.toInt();
}

/*! 网络套接字管理器析构函数
  */
TcpManager::~TcpManager()
{
    StopConnect();
    delete m_socket;
    m_socket = NULL;

    if(m_server != 0)
    {
        delete m_server;
    }
}

/*! 开始监听客户端的连接

    当发现有客户端连接的时候自动的调用对应的槽函数去处理

	\param ipaddr 绑定的ip地址字符串
	\
    \warning 该操作为异步
  */
void TcpManager::StartListen(QString &ipaddr)
{
    Q_ASSERT(m_socket == 0);
    m_server = new QTcpServer();

	bool bListenOk = false;

	if (ipaddr.isEmpty() == false)
	{
		bListenOk = m_server->listen(QHostAddress(ipaddr), m_port);
	}
	else
	{
		bListenOk = m_server->listen(QHostAddress::Any, m_port);
	}

    if(!m_server->isListening() && !bListenOk)
    {
        qDebug("TcpManager: %s Listening", qPrintable(objectName()));
    }
    else
    {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(AcceptNewUser()),Qt::UniqueConnection);
		emit this->preCheckFinished();
    }
}

/*! 开始连接服务端

    主动连接已经存在的服务端
    如果服务端还没有启动则连接失败

	\param ipaddr 主动连接的ip地址字符串
    \warning 该操作为异步
  */
void TcpManager::StartConnect(QString &ipaddr)
{
    if(m_socket != NULL)
    {
        if(m_socket->state() != QAbstractSocket::UnconnectedState)
        {
            m_socket->abort();
        }
    }

    m_socket = new QTcpSocket();
    connect(m_socket, SIGNAL(connected()), this,   SLOT(AcceptNewUser()));

	if (ipaddr.isEmpty() == true)
	{
		m_socket->connectToHost(QHostAddress::LocalHost, m_port);
	}else
	{
		m_socket->connectToHost(QHostAddress(ipaddr), m_port);
	}
}

/*! 检测当前的通讯是否建立了连接
  */
bool TcpManager::IsCOnnected()
{
    if(m_socket)
    {
        Q_ASSERT(m_socket->isOpen());
        Q_ASSERT(m_socket->isWritable());
        Q_ASSERT(m_socket->isReadable());
    }

    return (m_socket != 0) && (m_socket->state() == QTcpSocket::ConnectedState);
}

/*! 停止当前通讯连接
  */
void TcpManager::StopConnect()
{
    if(m_socket != 0)
    {
        m_socket->abort();
    }
}

/*! 保证连接建立后,创建通讯用户实体
    该实体会管理通讯中断和连接错误已经发送字节回馈等功能

    连接建立后,为连接标识生成一个固定的通讯用户,并且将其假如到通讯总线中.

    \warning 在加入总线之前需要互相发送自身标识,如果标识无效或者重复,都会导致失败.
  */

void TcpManager::AcceptNewUser()
{
    if(m_attri == "server")
    {
        m_socket = m_server->nextPendingConnection();
        m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        Q_ASSERT(m_socket->isOpen());
        Q_ASSERT(m_socket->isWritable());
        Q_ASSERT(m_socket->isReadable());
        Q_ASSERT(m_socket->isValid());
        //监听到一个socket之后马上的发送自己的guid过去
        m_socket->write(m_name.toAscii());
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ConnectError(QAbstractSocket::SocketError)));
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(UserLeave()));
        connect(m_socket, SIGNAL(bytesWritten(qint64)),
                this, SLOT(ByteWriten(qint64)));
    }
    else if(m_attri == "client")
    {
        m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        Q_ASSERT(m_socket->isOpen());
        Q_ASSERT(m_socket->isWritable());
        Q_ASSERT(m_socket->isReadable());
        Q_ASSERT(m_socket->isValid());
        m_socket->write(m_name.toAscii());
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ConnectError(QAbstractSocket::SocketError)));
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(UserLeave()));
        connect(m_socket, SIGNAL(bytesWritten(qint64)),
                this, SLOT(ByteWriten(qint64)));
    }

	QString uuid = ReceiveUuidName(m_socket, TcpFlag);
	bool addToBusSuccess = AddUserToBus((void*)m_socket, TcpFlag, uuid);

	if (addToBusSuccess == true)
	{
		if(m_attri == "server")
		{
			emit this->newClientConnectd(uuid);
		}
		else if(m_attri == "client")
		{
			emit this->connectToServer(uuid);
		}
	}else
	{
		this->addToBusFailed(uuid);
	}   
}

/*! 用户退出处理函数

    用户退出时会将其从总线中删除,并且通知上层当前是哪个用户断开
  */
void TcpManager::UserLeave()
{
    QTcpSocket* sc = qobject_cast<QTcpSocket*>(sender());

    if(sc != 0)
    {
        if(sc->isValid() == false)
        {
            return;
        }
    }
    else
    {
        return;
    }

    QString guid = DelUserFromBus((void*)sc,TcpFlag);

    //通知外部
    if(m_attri == "server")
    {
        emit this->clientDisconnected(guid);
    }
    else if(m_attri == "client")
    {
        emit this->serverDisconnected(guid);
    }
}

/*! 连接错误处理函数

    通知上层当前错误发生的代码

    \param err 错误码

  */
void TcpManager::ConnectError(QAbstractSocket::SocketError err)
{
    m_socket->close();
    m_socket = NULL;
    emit this->connectError(err);
}

/*! 写入字节报告函数

    通知上层当前已经传送的字节数

    \param len 写入字节数
  */
void TcpManager::ByteWriten(qint64 len)
{
    emit this->byteWriten(len);
}

QT_END_NAMESPACE
