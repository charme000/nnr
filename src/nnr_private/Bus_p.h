#ifndef BUS_P_H
#define BUS_P_H

#include "Bus.h"
#include "ProtocolFactory.h"
#include "PackageProtocol_1_0.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class ProtocolFactory;
class IProtocol;

class BusPrivate : public QObject
{
    Q_OBJECT
public:
    BusPrivate(Bus* bus);
    virtual ~BusPrivate() {}

public:
    void Send_p(QByteArray& data);
    void SendPackageToAll_p(QString& srcUuid, QByteArray& package, QByteArray& data);
    void SendPackageToOne_p(QString& dstUuid, QByteArray& package, QByteArray& data);

    bool Add_p(UserUnit& user, QString& guid);

    bool Delete_p(QString& uuid, Flag_Tag flag, bool immediate);
    bool Delete_p(UserUnit& user,Flag_Tag flag, bool immediate);
    void DeleteAll_p(Flag_Tag flagTag, bool immediate);

    QString GetGuid_p(UserUnit& user);
    QString GetGuid_p(void* value, Flag_Tag flag);
    UserUnit* GetUser_p(void *flag);
    UserUnit* GetUser_p(QString &uuid);

private:
    void CloseConnect(void* value, Flag_Tag flag, bool immediate);
public:
    IProtocol *m_p;
private:
    typedef QMap<QString, UserUnit*> userList;
    userList userMap;

    QUuid m_hostUuid;

private:
    Bus* const q_ptr;
    Q_DECLARE_PUBLIC(Bus)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // BUS_P_H
