#ifndef USERUNIT_P_H
#define USERUNIT_P_H

#include "UserUnit.h"
#include "IProvider.h"
#include "Bus.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class UserUnitPrivate : public QObject
{
    Q_OBJECT
public:
    UserUnitPrivate(UserUnit* u, Bus* bus, void* f, Flag_Tag flagTag);
    virtual ~UserUnitPrivate()
    {
        if(m_provider->m_protocol != NULL)
        {
            m_provider->unsetupReciveManager(m_provider->m_protocol);
        }
    }
public:
    void SendPackage_p(QByteArray& package);

private:
    UserUnit* const q_ptr;
    Q_DECLARE_PUBLIC(UserUnit)

private:
    QString m_package;
    IProvider* m_provider;
    Bus* m_bus;
    Flag_Tag m_ft;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // USERUNIT_P_H
