#ifndef IPROVIDER_H
#define IPROVIDER_H

#include "Common.h"
#include "Bus.h"
#include "ProtocolDefines.h"
#include "ProtocolFactory.h"
#include "IProtocol.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class IProtocol;

enum State_Tag
{
    Invalid = 0,
    WaitFlag,
    WaitData
};

/*! \class IProvider
    \brief 抽象传输接口
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

class NNR_EXPORT IProvider : public QObject
{
    Q_OBJECT
public:
    IProvider()
    {
        m_bus = NULL;
        m_protocol = NULL;
    }

    virtual void Send(QByteArray& b) {Q_UNUSED(b)}
    virtual void Send(QString& b) {Q_UNUSED(b)}

    void setupReciveManager(IProtocol* protocol)
    {
        connect(protocol, SIGNAL(recivedDataForAll(QString&, QByteArray&, QByteArray&)), m_bus, SLOT(RecivedDataForAll(QString&, QByteArray&, QByteArray&)), Qt::UniqueConnection);
        connect(protocol, SIGNAL(recivedDataForMe(QByteArray&, QByteArray&)), m_bus, SLOT(RecivedDataForMe(QByteArray&, QByteArray&)), Qt::UniqueConnection);
        connect(protocol, SIGNAL(recivedDataForOther(QString&, QByteArray&, QByteArray&)), m_bus, SLOT(RecivedDataForOther(QString&, QByteArray&, QByteArray&)), Qt::UniqueConnection);
    }

    void unsetupReciveManager(IProtocol* protocol)
    {
        disconnect(protocol, SIGNAL(recivedDataForAll(QString&, QByteArray&, QByteArray&)), m_bus, SLOT(RecivedDataForAll(QString&, QByteArray&, QByteArray&)));
        disconnect(protocol, SIGNAL(recivedDataForMe(QByteArray&, QByteArray&)), m_bus, SLOT(RecivedDataForMe(QByteArray&, QByteArray&)));
        disconnect(protocol, SIGNAL(recivedDataForOther(QString&, QByteArray&, QByteArray&)), m_bus, SLOT(RecivedDataForOther(QString&, QByteArray&, QByteArray&)));
    }
    void createProtocol(quint8 version)
    {
        QString str;

        if(version == 0x01)
        {
            str = _PROTOCOL_1_0;
        }
        else if(version == 0x02)
        {
            str = _PROTOCOL_2_0;
        }

        m_protocol = ProtocolFactory::Create(str);
    }
public:
    QByteArray m_data;
    Bus* m_bus;
    IProtocol* m_protocol;

public Q_SLOTS:
    void DataArrive() {}
};


QT_END_NAMESPACE

QT_END_HEADER

#endif // IPROVIDER_H
