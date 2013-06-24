#ifndef BUS_H
#define BUS_H

#include "Common.h"
#include "UserUnit.h"
#include "Defines.h"
#include "Macro.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class UserUnit;
class BusPrivate;
class IProtocol;

class NNR_EXPORT Bus : public QObject
{
    Q_OBJECT
public:
    explicit Bus();
    virtual ~Bus();

    QPointer<IProtocol> getProtocol();
    void setProtocol(QString& protocol);
    void setHostUuid(QString& host);
    QUuid getHostUuid();

    void Send(QByteArray& data);

    bool Add(UserUnit& user, QString& guid);

    bool Delete(QString& guid,Flag_Tag flag, bool immediate);
    bool Delete(UserUnit& user,Flag_Tag flag, bool immediate);
    void DeleteAll(Flag_Tag flag, bool immediate);

    QString GetGuid(UserUnit& user);
    QString GetGuid(void* value, Flag_Tag flag);
    UserUnit* GetUser(void* flag);
    UserUnit* GetUser(QString& uuid);

public Q_SLOTS:
    void RecivedDataForMe(QByteArray& package, QByteArray& data);
    void RecivedDataForOther(QString& dst, QByteArray& package, QByteArray& data);
    void RecivedDataForAll(QString& src, QByteArray& package, QByteArray& data);
Q_SIGNALS:
    void dataArrived(QByteArray& package, QByteArray& data);
protected:
    Bus(BusPrivate& d, QObject*);
private:
    BusPrivate* const d_ptr;
    //Q_DISABLE_COPY(Bus)
    Q_DECLARE_PRIVATE(Bus)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // BUS_H
