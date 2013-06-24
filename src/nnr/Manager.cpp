#include "Manager.h"

/*! \class Manager
    \brief 实现通讯中管理监听和接受等操作的接口
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

QT_BEGIN_NAMESPACE

/*! 构造函数
    \param bus 通讯总线
    \param attribute 通讯段属性
    \param value 通讯标志值
    \param guidName 标识uuid
  */
Manager::Manager(Bus& bus, QString& attribute, QString& value, QString &ip, QString& guidName):
    m_bus(NULL), m_user(NULL)
{
    m_start = value;
    m_attri = attribute;
    m_bus = &bus;
    m_name = guidName;
	m_ipaddr = ip;
}

/*! 析构函数
  */
Manager::~Manager()
{
    if(m_user != 0)
    {
        delete m_user;
    }
}

/*! 启动通信过程

    根据属性值启动通信过程,如果是server,则监听.如果是client,则连接.

    \sa StartListen(), StartConnect()
  */
void Manager::Start()
{
    if(m_attri == "server")
    {
        this->StartListen(m_ipaddr);
    }
    else if(m_attri == "client")
    {
        this->StartConnect(m_ipaddr);
    }
    else
    {
    }
}

/*! 获取互相之间传递的UUID标示符的字符串形式
	\param socket 标示连接的socket
	\param tag 标示当前所使用的连接的方式
 */

QString Manager::ReceiveUuidName(void* socket, Flag_Tag tag)
{
	QString uuidName;
	QByteArray name;

	if (m_useUuid == true)
	{
		if (tag == TcpFlag)
		{
			QLocalSocket *lsocket = (QLocalSocket*)socket;
			lsocket->waitForReadyRead();
			name = lsocket->readAll();
		}else if(tag == PipeFlag)
		{
			QTcpSocket *tsocket = (QTcpSocket*)socket;
			tsocket->waitForReadyRead();
			name = tsocket->readAll();
		}
		uuidName = QString(name);
	}else
	{
		uuidName = QString("user");
	}

	return uuidName;
}

/*! 获取当前启动标识值
  */
QString Manager::GetCurStartArg()
{
    return m_start;
}

/*! 获取当前通信端属性
  */
QString Manager::GetCurStartAttribute()
{
    return m_attri;
}

/*! 获取当前用户实体实例
  */
UserUnit* Manager::GetCurUser()
{
    return m_user;
}

/*! 获取当前通讯总线
  */
Bus* Manager::GetCurBus()
{
    return m_bus;
}

/*! 添加用户实体到通讯总线中

    \param user 实际通讯值
    \param ft 通讯类型
    \param guidName 标识uuid

    \sa DelUserFromBus(void* user, Flag_Tag ft)
  */
bool Manager::AddUserToBus(void* user, Flag_Tag ft, QString& guidName)
{
    bool ret = false;

    if((user != NULL) && (guidName.isEmpty() == false))
    {
        m_user = new UserUnit(m_bus,user, ft);
        ret = m_bus->Add(*m_user, guidName);
    }

    return ret;
}

/*! 从通讯总线中删除指定用户实体
    \param user 通讯实际值
    \param ft 通讯类型

    \sa AddUserToBus(void* user, Flag_Tag ft, QString& guidName)
  */
QString Manager::DelUserFromBus(void* user, Flag_Tag ft)
{
    QString guid;

    if(m_bus != NULL)
    {
        //获取该user的guid
        guid = m_bus->GetGuid(user, ft);

        if(guid.isEmpty() == false)
        {
            m_bus->Delete(guid,ft,true);
        }
    }

    return guid;
}

QT_END_NAMESPACE
