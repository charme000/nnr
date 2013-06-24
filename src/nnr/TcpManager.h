#ifndef TCPCONNECTOR_H
#define TCPCONNECTOR_H

#include "Manager.h"
#include "Bus.h"
#include "ProviderFactory.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class NNR_EXPORT TcpManager : public Manager
{
    Q_OBJECT
public:
    explicit TcpManager(Bus& bus, QString& attribute, QString& value, QString &ip, QString& guidName);
    ~TcpManager();

private:
	void StartConnect(QString &ipaddr);
	void StartListen(QString &ipaddr);
    bool IsCOnnected();
    void StopConnect();

public:
    int m_port;
    QTcpSocket* m_socket;
    QTcpServer* m_server;

public Q_SLOTS:
    void AcceptNewUser();
    void UserLeave();
    void ConnectError(QAbstractSocket::SocketError err);
    void ByteWriten(qint64 len);
};

QT_END_NAMESPACE

QT_END_HEADER


#endif // TCPCONNECTOR_H
