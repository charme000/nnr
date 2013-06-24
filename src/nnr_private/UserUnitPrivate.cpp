#include "UserUnit_p.h"
#include "ProviderFactory.h"

/*! \class UserUnitPrivate
    \brief 内部类
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

QT_BEGIN_NAMESPACE

UserUnitPrivate::UserUnitPrivate(UserUnit* u, Bus* bus, void* f, Flag_Tag flagTag):q_ptr(u),m_provider(NULL)
{
    if((flagTag != InvalidFlag))
    {
        m_provider =ProviderFactory::Create(bus, f,flagTag);
    }
}

void UserUnitPrivate::SendPackage_p(QByteArray& package)
{
    m_provider->Send(package);
}

QT_END_NAMESPACE

