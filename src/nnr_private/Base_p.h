#ifndef BASE_P_H
#define BASE_P_H

#include "Base.h"
#include "ManagerFactory.h"
#include "Manager.h"
#include "Bus.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class BasePrivate : public QObject
{
    Q_OBJECT
public:
    BasePrivate(Base* b, QObject* p);

    virtual ~BasePrivate() {}

public:
    typedef const char* SlotFunc;
    void Set_p(SlotFunc ConnectServerSuccess, SlotFunc AcceptNewClient, SlotFunc ConnectError, SlotFunc ServerDisconnected, SlotFunc ClientDisconnected, SlotFunc AddToBusFailed, SlotFunc DataArrived, SlotFunc ByteWriten,SlotFunc PreCheckFinished);
    void CreateManager_p(Bus& bus);

    void Start_p();
    bool Stop_p(QString& uuid,bool immediate);
    void StopAll(bool immediate);
private:
    Base* const q_ptr;

    QString m_attribute;
    Flag_Tag m_flag;
    QString m_value;
    QString m_guidname;
    Manager* m_manager;
    QObject* m_parent;
    Bus* m_bus;
	QString m_ip;

    Q_DECLARE_PUBLIC(Base)
};


QT_END_NAMESPACE

QT_END_HEADER

#endif // BASE_P_H
