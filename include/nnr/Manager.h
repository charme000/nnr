#ifndef MANAGER_H
#define MANAGER_H

#include "Common.h"
#include "Bus.h"
#include "UserUnit.h"
#include "nnr_global.h"

QT_BEGIN_NAMESPACE
class Bus;
class UserUnit;

class NNR_EXPORT Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(Bus& bus, QString& attribute, QString& value, QString &ip, QString& guidName);
    ~Manager();

public:
    void Start();
    virtual void StartListen(QString &ipaddr) = 0;
    virtual void StartConnect(QString &ipaddr) = 0;

    virtual bool IsCOnnected() = 0;
    virtual void StopConnect() = 0;

	QString ReceiveUuidName(void* socket, Flag_Tag tag);

    QString GetCurStartArg();

    QString GetCurStartAttribute();

    UserUnit* GetCurUser();

    Bus* GetCurBus();

    bool AddUserToBus(void* user, Flag_Tag ft, QString& guidName);

    QString DelUserFromBus(void* user, Flag_Tag ft);
public:
    QString m_start;
    QString m_attri;
    QString m_ori;
    Bus* m_bus;
    QString m_name;
    UserUnit* m_user;
	QString m_ipaddr;
	bool m_useUuid;

public Q_SLOTS:
    void AcceptNewUser() {}
    void UserLeave() {}
    void ConnectError(QAbstractSocket::SocketError err) {Q_UNUSED(err);}
    void ByteWriten(qint64 len) {Q_UNUSED(len);}

Q_SIGNALS:
    void addToBusFailed(QString uuid);
    void newClientConnectd(QString name);
    void connectToServer(QString name);
    void serverDisconnected(QString guid);
    void clientDisconnected(QString guid);
    void connectError(QAbstractSocket::SocketError err);
    void byteWriten(qint64 len);
	void preCheckFinished();
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // MANAGER_H
