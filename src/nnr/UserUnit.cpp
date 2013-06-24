#include "UserUnit.h"
#include "UserUnit_p.h"
#include "Bus.h"

/*! \class UserUnit
    \brief 参与连接以及通讯的实体定义类
    \version 1.0
    \date 2012.12.1-2012.1.30
    \pre 一般该类的方法不需要显式的调用,Bus类会递归或者单一的调用每个实体
  */

QT_BEGIN_NAMESPACE

/*! 通讯用户实体构造函数

    通知上层当前已经传送的字节数

    \param bus 通讯总线
    \param f 通讯标识
    \param flagTag 通信类型
  */
UserUnit::UserUnit(Bus* bus, void* f, Flag_Tag flagTag): d_ptr(new UserUnitPrivate(this, bus, f, flagTag)), m_f(NULL)
{
    if(f != NULL)
    {
        m_f = f;
    }
}

/*! 通讯用户实体析构函数
  */
UserUnit::~UserUnit()
{
    Q_D(UserUnit);
    delete d;
}

/*! 发送协议包

    \param package 根据指定协议类型生成的完整的协议包
  */
void UserUnit::SendPackage(QByteArray& package)
{
    Q_D(UserUnit);
    d->SendPackage_p(package);
}

QT_END_NAMESPACE
