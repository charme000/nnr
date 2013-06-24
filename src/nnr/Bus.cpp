#include "Bus.h"
#include "Bus_p.h"

/*! \class Bus
    \brief 实现数据通讯总线
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

QT_BEGIN_NAMESPACE

/*!
    构造函数
  */
Bus::Bus(): d_ptr(new BusPrivate(this))
{
}

/*!
    析构函数
  */
Bus::~Bus()
{
    Q_D(Bus);
    delete d;
}

/*!
    获取当前使用的协议接口指针

    \sa setProtocol(QString& protocol)
  */
QPointer<IProtocol> Bus::getProtocol()
{
    Q_D(Bus);
    return d->m_p;
}

/*!
    设置协议
    \param protocol 内置通讯协议的协议标识字符串为 \a _PROTOCOL_1_0

    通过传入的协议标识字符串,调用工厂来生产出符合类型的协议处理对象,该实例为单件模式.

    \sa _PROTOCOL_1_0,getProtocol()
  */
void Bus::setProtocol(QString& protocol)
{
    Q_D(Bus);
    d->m_p = ProtocolFactory::Create(protocol);
}

/*! 设置标识自身的uuid
    \param host 字符串形式的 \a uuid .例如: \a "{F85142D2-28D1-4F1E-9115-E1E2721F7B5B}"

    \sa getHostUuid()
  */
void Bus::setHostUuid(QString& host)
{
    Q_D(Bus);
    d->m_hostUuid = host;
}

/*! 获取标识自身的uuid
    \sa setHostUuid(QString& host)
  */
QUuid Bus::getHostUuid()
{
    Q_D(Bus);
    return d->m_hostUuid;
}

/*! 群发槽函数
    \param src 发包者的uuid
    \param package 协议包整体(此时的package中包含了附加数据)
    \param data 纯附加数据部分

    该函数会通过 \a bus 将接到的数据包依次转发出去

    \sa RecivedDataForMe(QByteArray& package, QByteArray& data),RecivedDataForOther(QString& dst, QByteArray& package, QByteArray& data)
  */
void Bus::RecivedDataForAll(QString& src, QByteArray& package, QByteArray& data)
{
    if(package.isEmpty() == false)
    {
        Q_D(Bus);
        d->SendPackageToAll_p(src, package, data);
    }
}

/*! 接收发给自己的数据包
    \param package 协议包整体(此时的package中包含了附加数据)
    \param data 纯附加数据部分

    \sa RecivedDataForAll(QString& src, QByteArray& package, QByteArray& data),RecivedDataForOther(QString& dst, QByteArray& package, QByteArray& data)
  */
void Bus::RecivedDataForMe(QByteArray& package, QByteArray& data)
{
    if(package.isEmpty() == false)
    {
        emit this->dataArrived(package, data);
    }
}

/*! 定向转发接到的数据包
    \param dst 目标uuid
    \param package 协议包整体(此时的package中包含了附加数据)
    \param data 纯附加数据部分

    \sa RecivedDataForMe(QByteArray& package, QByteArray& data),RecivedDataForAll(QString& src, QByteArray& package, QByteArray& data)
  */
void Bus::RecivedDataForOther(QString& dst, QByteArray& package, QByteArray& data)
{
    if(package.isEmpty() == false)
    {
        Q_D(Bus);
        d->SendPackageToOne_p(dst, package, data);
    }
}

/*! 发送数据
    \param data 纯附加数据(也可以为空)

    \warning 这里的data中不必要包含命令等其他的描述信息,描述信息通过其他的工具函数已经被指定

  */
void Bus::Send(QByteArray& data)
{
    Q_D(Bus);
    d->Send_p(data);
}

/*! 添加实体用户到通讯总线中

    \param user 用户实例
    \param guid 标识该用户的uuid字符串

    \warning 在添加用户到通讯总线中的时候,这两个参数都必须有效的,否则通讯过程中可能出现问题.
  */
bool Bus::Add(UserUnit& user, QString& guid)
{
    Q_D(Bus);
    return d->Add_p(user, guid);
}

/*! 根据uuid从bus中删除用户
    \param guid 带删除用户的uuid
    \param flag 用户通信类型(根据不同的类型来决定断开连接的方式)
    \param immediate 用户通信标记(决定立即断开还是等待数据接受完毕)

    \sa Delete(UserUnit& user, Flag_Tag flag, bool immediate),DeleteAll(Flag_Tag flag, bool immediate)
  */
bool Bus::Delete(QString& guid,Flag_Tag flag, bool immediate)
{
    Q_D(Bus);
    return d->Delete_p(guid,flag,immediate);
}

/*! 根据用户实例从bus中删除用户实体
    \param user 用户实例
    \param flag 用户通信类型(根据不同的类型来决定断开连接的方式)
    \param immediate 用户通信标记(决定立即断开还是等待数据接受完毕)

    \sa Delete(QString& guid,Flag_Tag flag, bool immediate),DeleteAll(Flag_Tag flag, bool immediate)
  */
bool Bus::Delete(UserUnit& user, Flag_Tag flag, bool immediate)
{
    Q_D(Bus);
    return d->Delete_p(user,flag,immediate);
}

/*! 删除当前bus中的所有用户实体
    \param flag 用户通信类型(根据不同的类型来决定断开连接的方式)
    \param immediate 用户通信标记(决定立即断开还是等待数据接受完毕)

    \sa Delete(UserUnit& user, Flag_Tag flag, bool immediate),Delete(QString& guid,Flag_Tag flag, bool immediate)
  */
void Bus::DeleteAll(Flag_Tag flag, bool immediate)
{
    Q_D(Bus);
    d->DeleteAll_p(flag,immediate);
}

/*! 获取指定用户实体的uuid
    \param user 用户实例的引用

    \sa GetGuid(void* value, Flag_Tag flag)
  */
QString Bus::GetGuid(UserUnit& user)
{
    Q_D(Bus);
    return d->GetGuid_p(user);
}

/*! 根据通信类型和通信值来查找uuid
    \param value 通信中的实际值(比如socket)
    \param flag 用户通信类型(根据不同的类型来决定断开连接的方式)

    \sa GetGuid(UserUnit& user)
  */
QString Bus::GetGuid(void* value, Flag_Tag flag)
{
    if((value != NULL) || (flag == InvalidFlag))
    {
        Q_D(Bus);
        return d->GetGuid_p(value, flag);
    }

    return EMPTY;
}

/*! 根据uuid获取对应的用户实体实例
    \param uuid 标识用户的uuid

    \sa GetUser(void* flag)
  */
UserUnit* Bus::GetUser(QString& uuid)
{
    Q_D(Bus);
    return d->GetUser_p(uuid);
}

/*! 根据具体的通信值来获取用户实体实例
    \param flag 通信中的实际值(比如socket)

    \sa GetUser(QString& uuid)
  */
UserUnit* Bus::GetUser(void* flag)
{
    Q_D(Bus);
    return d->GetUser_p(flag);
}


QT_END_NAMESPACE
