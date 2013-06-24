#ifndef RPCPROTOCOL_H
#define RPCPROTOCOL_H

#include "IProtocol.h"
#include "ProtocolDefines.h"
#include "nnr_global.h"

#define RPCPTC_HEADER_SIZE 0x2
#define RPCPTC_BODY_SIZE 0x25
#define RPCPTC_INFO_SIZE 0x27

/*! \class _PACKAGE_UUID
    \brief 内置通讯协议UUID标识定义
    \version 1.0
    \date 2012.12.1-2012.1.30
  */
typedef struct _PACKAGE_UUID
{
    quint32 data1;
    quint16 data2;
    quint16 data3;
    quint8 data4[8];
} PACKAGE_UUID, *PPACKAGE_UUID;

/*! \class _PROTOCOL_HEADER_
    \brief 内置通讯协议协议头定义
    \version 1.0
    \date 2012.12.1-2012.1.30
  */
typedef struct _PROTOCOL_HEADER_
{
    quint8 Sign; // 协议标识
    quint8 Check; // 协议版本
} PROTOCOL_HEADER, *PPROTOCOL_HEADER;

/*! \class _PACKAGE_BODY_
    \brief 内置通讯协议数据定义
    \version 1.0
    \date 2012.12.1-2012.1.30
  */
typedef struct _PACKAGE_BODY_
{
    quint32 Cmd; // 命令
    quint8 Type;
    PACKAGE_UUID SrcAddr; //源地址
    PACKAGE_UUID DesAddr; //目标地址

} PACKAGE_BODY, *PPACKAGE_BODY;

/*! \class _PROTOCOL_INFO_
    \brief 内置通讯协议包定义
    \version 1.0
    \date 2012.12.1-2012.1.30
  */
typedef struct _PROTOCOL_INFO_
{
    PROTOCOL_HEADER Header;
    PACKAGE_BODY Body;
} PROTOCOL_INFO, *PPROTOCOL_INFO;

class NNR_EXPORT PackageProtocol_1_0 : public IProtocol
{
    Q_OBJECT
public:
	static PackageProtocol_1_0* GetInstance()
	{
		if(m_pIns == NULL)
		{
			m_pIns = new PackageProtocol_1_0();
		}
		return m_pIns;
	}
private:
	static PackageProtocol_1_0* m_pIns;
	class Garbo
	{
	public:
		~Garbo()
		{
			if(m_pIns)
			{
				delete m_pIns;
			}
		}
	};
	static Garbo gb;
public:
    PackageProtocol_1_0()
    {
        m_type = PKG_TYPE_NONE;
        m_cmd = 0;
        m_srcUuid = QString("");
        m_dstUuid = QString("");
    }

    void setSrc(QUuid& Src);
    void setDst(QUuid& Dst);
    void setCmd(quint32 Cmd);
    void setType(quint8 Type);

    QUuid getSrc(QByteArray& package);
    QUuid getDst(QByteArray& package);
    quint32 getCmd(QByteArray& package);
    quint8 getType(QByteArray& package);

    QByteArray& GetPackage();
    bool AnalysisPackage(QByteArray& data);
    bool DataRecived(QDataStream& stream, quint32 len, QString self);
private:
    quint32 GetCmdValue() {return 0;}
    void setCmdValue(quint32 newCmd) {Q_UNUSED(newCmd);return;}
    quint32 GetPackageCount() {return 0;}
    void setPackageCount(quint32 newPackageCount) {Q_UNUSED(newPackageCount);return;}
    quint32 GetPackageDataSize() {return 0;}
    void setPackageDataSize(quint32 newPackageDataSize) {Q_UNUSED(newPackageDataSize);return;}
    quint8 GetPackageType();
    void SetPackageType(quint8 type);
    QString GetSrcUuid();
    void setSrcUuid(QString newSrcUuid) {return;}
    QString GetDstUuid();
    void setDstUuid(QString newDstUuid) {return;}
    void FillInternalBuffer(QByteArray& data) {Q_UNUSED(data);return;}
    void DispatchPackage(QString& self, char* data, quint32& dataSize);
    void MakePackage(QDataStream& stream);
    int ReleasePackage(QDataStream& stream);
private:
    QUuid m_srcUuid;
    QUuid m_dstUuid;
    quint8 m_type ;
    quint32 m_cmd;
};

inline void PackageProtocol_1_0::setCmd(quint32 Cmd)
{
    m_cmd = Cmd;
}

inline void PackageProtocol_1_0::setDst(QUuid& Dst)
{
    m_dstUuid = Dst;
}

inline void PackageProtocol_1_0::setSrc(QUuid& Src)
{
    m_srcUuid = Src;
}

inline void PackageProtocol_1_0::setType(quint8 type)
{
    m_type = type;
}



#endif // RPCPROTOCOL_H


