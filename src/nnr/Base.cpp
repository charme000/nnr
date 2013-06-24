#include "Base.h"
#include "..\nnr_private\Base_p.h"
#include "Bus.h"

/*! \class Base
    \brief 提供异步多客户端通信的管理及使用等调用接口
    \version 1.0
    \date 2012.12.1-2012.1.30
    \pre 实例化该类之后,必须调用set类函数进行相关设置,之后
    调用Set函数设置处理回调,最后调用 Start 和 Stop* 手工启动和关闭通讯
  */

QT_BEGIN_NAMESPACE
/*!
    通过指定的父对象来构造一个\a Base管理对象
  */
Base::Base(QObject* parent)
    : d_ptr(new BasePrivate(this, parent))
{
}

/*! 销毁管理对象
  */
Base::~Base()
{
    Q_D(Base);
    delete d;
}

/*! 启动
  */
void Base::Start()
{
    Q_D(Base);
    d->Start_p();
}

/*! 通过\a uuid 和指定的 \a immediate 选项来断开某个连接

    \param immediate 决定在断开连接的过程中是立即断开还是等待数据接受完毕后断开

    \param uuid 匹配当前 \a Bus 中的 \a uuid 找到对应的 \a UserUnit
    从而断开连接,删除对象,最后从总线中移除对应条目.断开连接动作执行之后会接收到 \a clientDisconnected 或者 \a serverDisconnected 信号

    \sa StopAll(bool immediate)
  */
bool Base::Stop(QString& uuid,bool immediate)
{
    Q_D(Base);
    return d->Stop_p(uuid,immediate);
}

/*! 断开当前 \a Bus 中所有 \a UserUnit 的连接

    \param immediate 决定在断开连接的过程中是立即断开还是等待数据接受完毕后断开

    \sa Stop(QString &uuid,bool immediate)
  */
void Base::StopAll(bool immediate)
{
    Q_D(Base);
    d->StopAll(immediate);
}

/*! 创建管理器

    \param bus 供每个管理器管理bus中的元素
  */
void Base::CreateManager(Bus& bus)
{
    Q_D(Base);
    d->CreateManager_p(bus);
}

/*! 设置回调处理函数

    \param ConnectServerSuccess 成功连接到服务端时触发
    \param AcceptNewClient 成功接受到客户端连接时触发
    \param ConnectError 连接过程中发生错误时触发
    \param ServerDisconnected 服务端退出时触发
    \param ClientDisconnected 任一客户端退出时触发
    \param AddToBusFailed 添加用户实体到Bus中时如果对应实体已经存在时触发
    \param DataArrived 有数据到达时触发
    \param ByteWriten 主动发送数据写入完毕时触发

    \warning 以上参数如果不需要处理相应的动作,则可以设置为NULL.以上动作都为异步动作.
  */
void Base::Set(SlotFunc ConnectServerSuccess, SlotFunc AcceptNewClient, SlotFunc ConnectError, SlotFunc ServerDisconnected, SlotFunc ClientDisconnected, SlotFunc AddToBusFailed, SlotFunc DataArrived, SlotFunc ByteWriten, SlotFunc PreCheckFinished)
{
    Q_D(Base);
    d->Set_p(ConnectServerSuccess, AcceptNewClient, ConnectError, ServerDisconnected, ClientDisconnected, AddToBusFailed,DataArrived, ByteWriten,PreCheckFinished);
}

/*! 设置当前管理端的属性
    \param strAttribute 当前只支持\a _CLIENT_ATTRIBUTE_ 和 \a _SERVER_ATTRIBUTE_ 两种

    \sa attribute(),_CLIENT_ATTRIBUTE_,_SERVER_ATTRIBUTE_
  */
void Base::setAttribute(const QString& strAttribute)
{
    Q_D(Base);
    d->m_attribute = strAttribute;
}

/*! 设置当前管理端的UUID标识
    \param strName 输入字符串形式的Uuid.例如: \a "{F85142D2-28D1-4F1E-9115-E1E2721F7B5B}"

    \sa uuid()
  */
void Base::setUuidName(const QString& strName)
{
    Q_D(Base);
    d->m_guidname = strName;
}

/*! 设置通讯类型
    \param flag 通讯类型(目前只支持网络套接字和本地套接字两种)

    \sa Flag_Tag,flag()
  */
void Base::setFlag(Flag_Tag flag)
{
    Q_D(Base);
    d->m_flag = flag;
}

/*! 设置通讯标记
    \param strValue 如果使用本地套接字类型则可以传入任意的字符串,如果选择网络套接字类型则需要传入有效的端口号

    \sa v()
  */
void Base::setValue(const QString& strValue)
{
    Q_D(Base);
    d->m_value = strValue;
}

/*! 设置通讯IP
    \param strIp

    \sa ip()
  */
void Base::setIp(const QString& strIp)
{
	Q_D(Base);
	d->m_ip = strIp;
}

/*! 设置是否只用标示
    \param bUse

    \sa useUuid()
  */
void Base::setUseUuid(bool bUse)
{
	Q_D(Base);
	d->m_manager->m_useUuid = bUse;
}

/*!
    \sa setAttribute()
  */
QString Base::attribute()
{
    Q_D(Base);
    return  d->m_attribute;
}

/*!
    \sa setFlag()
  */
Flag_Tag Base::flag()
{
    Q_D(Base);
    return  d->m_flag;
}

/*!
    \sa setValue()
  */
QString Base::v()
{
    Q_D(Base);
    return  d->m_value;
}

/*!
    \sa setUuid()
  */
QString Base::uuid()
{
    Q_D(Base);
    return  d->m_guidname;
}

/*!
    \sa setIp()
  */
QString Base::ip()
{
	Q_D(Base);
	return d->m_ip;
}

/*!
	\sa setUseUuid()
 */
bool Base::useUuid()
{
	Q_D(Base);
	return d->m_manager->m_useUuid;
}

QT_END_NAMESPACE
