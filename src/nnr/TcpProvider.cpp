#include "TcpProvider.h"

/*! \class TcpProvider
    \brief 利用网络套接字来实现传输和接受数据的接口
    \version 1.0
    \date 2012.12.1-2012.1.30
    \pre 一般该类的方法不需要显式的调用,Bus类会整体的调用并且管理
  */

QT_BEGIN_NAMESPACE
static bool hasRecivedHeader = false;

/*! 发送数据

    \param b 待发送数据
  */
void TcpProvider::Send(QByteArray& b)
{
    if(m_socket->isValid())
    {
        //qtcpsocet在写的时候先是填满自己内部的缓冲才一并发的 所以一次可能发不出去
        int nSize = m_socket->write(b);

        while(nSize < b.size())
        {
            nSize += m_socket->write(b.right(b.size() - nSize));
        }
    }
}

/*! 接受处理函数

    当有数据到达时会通过指定的协议去解析接收到的数据

    \warning 目前不支持拆包接收
  */
void TcpProvider::DataArrive()
{
    QDataStream OutDataStream(m_socket);
    OutDataStream.setVersion(QDataStream::Qt_4_7);
    State_Tag state = WaitFlag;

    while(true)
    {
        switch(state)
        {
            case WaitFlag:
            {
                if(hasRecivedHeader == false)
                {
                    qint64 len = m_socket->bytesAvailable();

                    if(len < PG_HEADER_LEN)
                    {
                        return;
                    }

                    quint8 curFlag = 0;
                    quint8 curVersion = 0;
                    OutDataStream >> curFlag >> curVersion;

                    //判断标识
                    if(curFlag != PG_SIGN)
                    {
                        return;
                    }

                    if(curVersion != PG_VER_INVALID)
                    {
                        //根据版本生产出工厂去处理接收到的包
                        //首先按照版本创建协议工厂 因为它要处理和分析接收到的包
                        createProtocol(curVersion);
                        setupReciveManager(m_protocol);
                        hasRecivedHeader = true;
                        state = WaitData;
                    }
                    else
                    {
                        return;
                    }
                }

                break;
            }
            case WaitData:
            {
                bool ret = m_protocol->DataRecived(OutDataStream, m_socket->bytesAvailable(), m_bus->getHostUuid().toString());

                if(ret == true)
                {
                    hasRecivedHeader = false;
                    state = WaitFlag;
                }

                break;
            }
            case Invalid:
            {
                break;
            }
        }
    }
}

QT_END_NAMESPACE
