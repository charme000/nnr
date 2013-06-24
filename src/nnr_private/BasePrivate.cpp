#include "Base_p.h"

/*! \class BasePrivate
    \brief 内部类
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

QT_BEGIN_NAMESPACE

BasePrivate::BasePrivate(Base* b, QObject* p): q_ptr(b)
{
    m_parent = p;
}

void BasePrivate::Start_p()
{
    if(m_manager != NULL)
    {
        m_manager->Start();
    }
}

bool BasePrivate::Stop_p(QString& uuid, bool immediate)
{
    bool ret = false;

    if(m_bus != NULL)
    {
        ret = m_bus->Delete(uuid,m_flag,immediate);
    }

    return ret;
}

void BasePrivate::StopAll(bool immediate)
{
    if(m_bus != NULL)
    {
        m_bus->DeleteAll(m_flag,immediate);
    }
}

void BasePrivate::Set_p(SlotFunc ConnectServerSuccess, SlotFunc AcceptNewClient, SlotFunc ConnectError, SlotFunc ServerDisconnected, SlotFunc ClientDisconnected, SlotFunc AddToBusFailed, SlotFunc DataArrived, SlotFunc ByteWriten,SlotFunc PreCheckFinished)
{
    if(m_bus)
    {
        if(DataArrived)
        {
            connect(m_bus, SIGNAL(dataArrived(QByteArray&,QByteArray&)), m_parent, DataArrived);
        }
    }

    if(m_manager)
    {
        if(ConnectServerSuccess)
        {
            connect(m_manager, SIGNAL(connectToServer(QString)), m_parent, ConnectServerSuccess);
        }

        if(AddToBusFailed)
        {
            connect(m_manager, SIGNAL(addToBusFailed(QString)), m_parent, AddToBusFailed);
        }

        if(AcceptNewClient)
        {
            connect(m_manager, SIGNAL(newClientConnectd(QString)), m_parent, AcceptNewClient);
        }

        if(ConnectError)
        {
            connect(m_manager, SIGNAL(connectError(QAbstractSocket::SocketError)), m_parent, ConnectError);
        }

        if(ServerDisconnected)
        {
            connect(m_manager, SIGNAL(serverDisconnected(QString)), m_parent, ServerDisconnected);
        }

        if(ClientDisconnected)
        {
            connect(m_manager, SIGNAL(clientDisconnected(QString)), m_parent, ClientDisconnected);
        }

        if(ByteWriten)
        {
            connect(m_manager, SIGNAL(byteWriten(qint64)), m_parent, ByteWriten);
        }
		if(PreCheckFinished)
		{
			connect(m_manager, SIGNAL(preCheckFinished()), m_parent, PreCheckFinished);
		}
    }
}

void BasePrivate::CreateManager_p(Bus& bus)
{
    m_bus = &bus;
    m_bus->setHostUuid(m_guidname);
    m_manager = ManagerFactory::Create(bus, m_attribute, m_flag, m_value, m_ip, m_guidname);
}

QT_END_NAMESPACE
