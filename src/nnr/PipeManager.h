#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include "Manager.h"
#include "Bus.h"
#include "ProviderFactory.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class NNR_EXPORT PipeManager : public Manager
{
    Q_OBJECT
public:
    explicit PipeManager(Bus& bus, QString& attribute, QString& value, QString &ip, QString& guidName);
    ~PipeManager();

private:
    void StartConnect(QString &ipaddr);
    void StartListen(QString &ipaddr);
    bool IsCOnnected();
    void StopConnect();

public:

    QLocalSocket* m_socket;
    QLocalServer* m_server;
    QString m_pipeName;

public Q_SLOTS:
    void AcceptNewUser();
    void UserLeave();
    void ConnectError(QAbstractSocket::SocketError err);
    void ByteWriten(qint64 len);
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // PIPEMANAGER_H
