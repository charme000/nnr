#include "ProtocolFactory.h"

/*! \class ProtocolFactory
    \brief 协议工厂类
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

PackageProtocol_1_0* PackageProtocol_1_0::m_pIns = NULL;

/*! 协议工厂生产函数
    \param protocolType 协议类型(字符串标识,例如:"nnr_1_0" 表示使用1.0协议)

    \warning 生产过程为单件模式
  */
IProtocol* ProtocolFactory::Create(QString& protocolType)
{
    QPointer<IProtocol> p;

    if(protocolType == _PROTOCOL_1_0)
    {
        p = PackageProtocol_1_0::GetInstance();
    }
    else if(protocolType == _PROTOCOL_2_0)
    {
    }
    else
    {
    }

    return p;
}
