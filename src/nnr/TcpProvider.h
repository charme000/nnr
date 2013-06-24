#ifndef TCPPROVIDER_H
#define TCPPROVIDER_H

#include "Common.h"
#include "IProvider.h"
#include "UserUnit.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class NNR_EXPORT TcpProvider : public IProvider
{
    Q_OBJECT

public:
    TcpProvider(QTcpSocket* s)
    {
        m_socket =  s;
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(DataArrive()), Qt::UniqueConnection);
    }

    ~TcpProvider()
    {
        disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(DataArrive()));
        delete m_socket;
        m_socket = NULL;
    }

    void Send(QByteArray& b);

private:
    QTcpSocket* m_socket;

public Q_SLOTS:
    void DataArrive();
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // TCPPROVIDER_H
