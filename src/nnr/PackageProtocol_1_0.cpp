#include "PackageProtocol_1_0.h"

/*! \class PackageProtocol_1_0
    \brief 内置通讯协议
    \version 1.0
    \date 2012.12.1-2012.1.30
    \pre 内置通讯协议不支持大数据包的拆解,但是支持数据包的定向转发和群发.
    其它协议的命名需要遵循PackageProtocol_x_y 的格式,以保持调用接口的统一
  */

QT_BEGIN_NAMESPACE
static PACKAGE_BODY m_body;
static PROTOCOL_INFO m_info;

QDataStream& operator<<(QDataStream& stream, const PROTOCOL_HEADER& data)
{
    stream << data.Sign;
    stream << data.Check;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, PROTOCOL_HEADER& data)
{
    stream >> data.Sign;
    stream >> data.Check;
    return stream;
}

QDataStream& operator<<(QDataStream& stream, const PACKAGE_UUID& data)
{
    stream << data.data1;
    stream << data.data2;
    stream << data.data3;
    stream << data.data4[0];
    stream << data.data4[1];
    stream << data.data4[2];
    stream << data.data4[3];
    stream << data.data4[4];
    stream << data.data4[5];
    stream << data.data4[6];
    stream << data.data4[7];
    return stream;
}

QDataStream& operator>>(QDataStream& stream, PACKAGE_UUID& data)
{
    stream >> data.data1;
    stream >> data.data2;
    stream >> data.data3;
    stream >> data.data4[0];
    stream >> data.data4[1];
    stream >> data.data4[2];
    stream >> data.data4[3];
    stream >> data.data4[4];
    stream >> data.data4[5];
    stream >> data.data4[6];
    stream >> data.data4[7];
    return stream;
}

QDataStream& operator<<(QDataStream& stream, const PACKAGE_BODY& data)
{
    stream << data.Cmd;
    stream << data.Type;
    stream << data.SrcAddr;
    stream << data.DesAddr;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, PACKAGE_BODY& data)
{
    stream >> data.Cmd;
    stream >> data.Type;
    stream >> data.SrcAddr;
    stream >> data.DesAddr;
    return stream;
}

QDataStream& operator<<(QDataStream& stream, const PROTOCOL_INFO& data)
{
    stream << data.Header;
    stream << data.Body;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, PROTOCOL_INFO& data)
{
    stream >> data.Header;
    stream >> data.Body;
    return stream;
}

/*! 获取协议包中的源uuid(供外部调用)

    \param package 完整的协议包

    \sa getDst(QByteArray& package)
  */
QUuid PackageProtocol_1_0::getSrc(QByteArray& package)
{
    PROTOCOL_INFO info;
    QDataStream stream(&package,QIODevice::ReadOnly);
    stream >> info;
    QUuid src(info.Body.SrcAddr.data1, info.Body.SrcAddr.data2, info.Body.SrcAddr.data3, info.Body.SrcAddr.data4[0], info.Body.SrcAddr.data4[1], info.Body.SrcAddr.data4[2], info.Body.SrcAddr.data4[3], info.Body.SrcAddr.data4[4], info.Body.SrcAddr.data4[5], info.Body.SrcAddr.data4[6], info.Body.SrcAddr.data4[7]);
    return src;
}

/*! 获取协议包中的目标uuid(供外部调用)

    \param package 完整的协议包

    \sa getSrc(QByteArray& package)
  */
QUuid PackageProtocol_1_0::getDst(QByteArray& package)
{
    PROTOCOL_INFO info;
    QDataStream stream(&package,QIODevice::ReadOnly);
    stream >> info;
    QUuid dst(info.Body.DesAddr.data1, info.Body.DesAddr.data2, info.Body.DesAddr.data3, info.Body.DesAddr.data4[0], info.Body.DesAddr.data4[1], info.Body.DesAddr.data4[2], info.Body.DesAddr.data4[3], info.Body.DesAddr.data4[4], info.Body.DesAddr.data4[5], info.Body.DesAddr.data4[6], info.Body.DesAddr.data4[7]);
    return dst;
}

/*! 获取协议包中的命令字段(供外部调用)

    \param package 完整的协议包

  */
quint32 PackageProtocol_1_0::getCmd(QByteArray& package)
{
    PROTOCOL_INFO info;
    QDataStream stream(&package,QIODevice::ReadOnly);
    stream >> info;
    return info.Body.Cmd;
}

/*! 获取协议包中的包类型字段(供外部调用)

    \param package 完整的协议包

  */
quint8 PackageProtocol_1_0::getType(QByteArray& package)
{
    PROTOCOL_INFO info;
    QDataStream stream(&package,QIODevice::ReadOnly);
    stream >> info;
    return info.Body.Type;
}

/*! 生成协议包

    \param stream 流通道实例

    \sa ReleasePackage(QDataStream& stream)
  */
void PackageProtocol_1_0::MakePackage(QDataStream& stream)
{
    m_body.Cmd = m_cmd;
    m_body.Type = m_type;
    m_body.SrcAddr.data1 = m_srcUuid.data1;
    m_body.SrcAddr.data2 = m_srcUuid.data2;
    m_body.SrcAddr.data3 = m_srcUuid.data3;
    memcpy(&m_body.SrcAddr.data4, &m_srcUuid.data4, 8);
    m_body.DesAddr.data1 = m_dstUuid.data1;
    m_body.DesAddr.data2 = m_dstUuid.data2;
    m_body.DesAddr.data3 = m_dstUuid.data3;
    memcpy(&m_body.DesAddr.data4, &m_dstUuid.data4, 8);
    //填充协议头
    m_info.Header.Sign = PG_SIGN;
    m_info.Header.Check = PG_VER_01;
    //填充协议内容
    m_info.Body = m_body;
    //写入到stream
    stream << m_info;
}

/*! 输入数据包到结构体中

    \param stream 流通道实例

    \sa MakePackage(QDataStream& stream)
  */
int PackageProtocol_1_0::ReleasePackage(QDataStream& stream)
{
    //写入到body
    stream >> m_body;
    return 0;
}

/*! 获取当前生成好的协议包
  */
QByteArray& PackageProtocol_1_0::GetPackage()
{
    return m_Package;
}

/*! 根据传入的原始数据分析生成协议包的细节
    \param data 原始附加数据
  */
bool PackageProtocol_1_0::AnalysisPackage(QByteArray& data)
{
    QDataStream stream(&m_Package, QIODevice::WriteOnly);
    MakePackage(stream);
    stream << data;
    memset(&m_info, 0, sizeof(PROTOCOL_INFO));
    memset(&m_body, 0, sizeof(PACKAGE_BODY));

    if(m_Package.isEmpty() == true)
    {
        return false;
    }

    return true;
}

/*! 获取协议包中的包类型(供内部调用)
  */
quint8 PackageProtocol_1_0::GetPackageType()
{
    return m_info.Body.Type;
}

/*! 设置协议包中的包类型(供内部调用)
  */
void PackageProtocol_1_0::SetPackageType(quint8 type)
{
    m_info.Body.Type = type;
}

/*! 获取协议包中的源uuid(供内部调用)
  */
QString PackageProtocol_1_0::GetSrcUuid()
{
    QUuid uuid(m_info.Body.SrcAddr.data1, m_info.Body.SrcAddr.data2, m_info.Body.SrcAddr.data3, m_info.Body.SrcAddr.data4[0], m_info.Body.SrcAddr.data4[1], m_info.Body.SrcAddr.data4[2], m_info.Body.SrcAddr.data4[3], m_info.Body.SrcAddr.data4[4], m_info.Body.SrcAddr.data4[5], m_info.Body.SrcAddr.data4[6], m_info.Body.SrcAddr.data4[7]);
    return uuid.toString();
}

/*! 获取协议包中的目的uuid(供内部调用)
  */
QString PackageProtocol_1_0::GetDstUuid()
{
    QUuid uuid(m_info.Body.DesAddr.data1, m_info.Body.DesAddr.data2, m_info.Body.DesAddr.data3, m_info.Body.DesAddr.data4[0], m_info.Body.DesAddr.data4[1], m_info.Body.DesAddr.data4[2], m_info.Body.DesAddr.data4[3], m_info.Body.DesAddr.data4[4], m_info.Body.DesAddr.data4[5], m_info.Body.DesAddr.data4[6], m_info.Body.DesAddr.data4[7]);
    return uuid.toString();
}

/*! 派发数据包
    \param self 标识自身的uuid
    \param data 附加数据
    \param 附加数据大小

    \warning 派发的逻辑有三种:发给自身,群发,定向发给某端
  */
void PackageProtocol_1_0::DispatchPackage(QString& self, char* data, quint32& dataSize)
{
    //首先判断包类型 如果是群发包那么就直接给bus
    //如果是定向包的话 是定向还给谁的
    //给自身的 直接一层一层传递
    //给某人的则给bus
    //拷贝数据 如果参数中的data指针被释放数据也还在
    QByteArray package;
    QDataStream stream(&package, QIODevice::WriteOnly);
    QByteArray byte;

    if(data != NULL)
    {
        byte.append(data, (int)dataSize);
    }

    quint8 packageType = GetPackageType();

    if(packageType == PKG_TYPE_BROAD)
    {
        //重新设置包类型 保证不造成死循环
        SetPackageType(PKG_TYPE_NONE);
        QString src = GetSrcUuid();
        stream << m_info;
        //转发所有
        emit this->recivedDataForAll(src, package, byte);
        //发给自身
        emit this->recivedDataForMe(package, byte);
    }
    else if(packageType == PKG_TYPE_DIRECT)
    {
        QString dst = GetDstUuid();
        stream << m_info;

        if(dst == self)
        {
            //发给自己的 通知上层
            emit this->recivedDataForMe(package, byte);
        }
        else
        {
            //不是发给自己的 那么是发给谁的 要给bud 目标id
            emit this->recivedDataForOther(dst, package, byte);
        }
    }
    else if(packageType == PKG_TYPE_NONE)
    {
        stream << m_info;
        //发给自己的 通知上层
        emit this->recivedDataForMe(package, byte);
    }
    else
    {
    }

    package.clear();
    byte.clear();
}

/*! 接受到数据后解析
    \param stream 流管道
    \param len 当前到达数据的长度
    \param self 标识自身的uuid
  */
bool PackageProtocol_1_0::DataRecived(QDataStream& stream, quint32 len, QString self)
{
    quint32 tmplen = len;
    quint32 dataLen = 0;
    char* pdata = NULL;

    if(tmplen >= RPCPTC_BODY_SIZE)
    {
        tmplen -= RPCPTC_BODY_SIZE;
        ReleasePackage(stream);

        if(tmplen >= 4)
        {
            stream >> dataLen;
            tmplen -= 4;

            if(tmplen >= dataLen)
            {
                if(dataLen != 0)
                {
                    pdata = new char[dataLen];
                    stream.readRawData(pdata, dataLen);
                }

                //此处合并为一个完整的包
                m_info.Body = m_body;
                m_info.Header.Sign = PG_SIGN;
                m_info.Header.Check = PG_VER_01;
                DispatchPackage(self, pdata, dataLen);
                return true;
            }
        }
    }

    return false;
}
