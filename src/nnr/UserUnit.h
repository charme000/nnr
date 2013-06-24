#ifndef USERUNIT_H
#define USERUNIT_H

#include "Common.h"
#include "Defines.h"
#include "Macro.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class UserUnitPrivate;
class Bus;

class NNR_EXPORT UserUnit : public QObject
{
    Q_OBJECT
public:
    UserUnit(Bus* bus, void* f, Flag_Tag flagTag);
    virtual ~UserUnit();

public:
    void SendPackage(QByteArray& package);
protected:
    UserUnit(UserUnitPrivate& d);

private:
    UserUnitPrivate* const d_ptr;
    Q_DISABLE_COPY(UserUnit)
    Q_DECLARE_PRIVATE(UserUnit)

public:
    void* m_f;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // USERUNIT_H
