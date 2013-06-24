#ifndef PIPEPROVIDER_H
#define PIPEPROVIDER_H

#include "Common.h"
#include "IProvider.h"
#include "UserUnit.h"
#include "PackageProtocol_1_0.h"
#include "ProtocolFactory.h"
#include "nnr_global.h"
#include <QMutex>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class ProtocolFactory;

class NNR_EXPORT PipeProvider : public IProvider
{
    Q_OBJECT

public:
    PipeProvider(QLocalSocket* s)
    {
        m_socket =  s;
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(DataArrive()), Qt::UniqueConnection);
    }

    ~PipeProvider()
    {
        disconnect(m_socket,SIGNAL(readyRead()),this,SLOT(DataArrive()));
        delete m_socket;
        m_socket = NULL;
    }

    void Send(QByteArray& b);

private:
    QLocalSocket* m_socket;

public Q_SLOTS:
    void DataArrive();
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // PIPEPROVIDER_H
