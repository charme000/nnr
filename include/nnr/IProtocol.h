#ifndef IPROTOCOL_H
#define IPROTOCOL_H

#include "Common.h"
#include "nnr_global.h"

#define _ACCEPT_SELF_ 1
QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
/*! \class IProtocol
    \brief 抽象协议接口
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

class NNR_EXPORT IProtocol : public QObject
{
    Q_OBJECT
public:
    virtual quint32 GetCmdValue() = 0;
    virtual void setCmdValue(quint32 newCmd) = 0;
    virtual quint32 GetPackageCount() = 0;
    virtual void setPackageCount(quint32 newPackageCount) = 0;
    virtual quint32 GetPackageDataSize() = 0;
    virtual void setPackageDataSize(quint32 newPackageDataSize) = 0;
    virtual quint8 GetPackageType() = 0;
    virtual void SetPackageType(quint8 type) = 0;
    virtual QString GetSrcUuid() = 0;
    virtual void setSrcUuid(QString newSrcUuid) = 0;
    virtual QString GetDstUuid() = 0;
    virtual void setDstUuid(QString newDstUuid) = 0;


    virtual QByteArray& GetPackage() = 0;
    virtual bool AnalysisPackage(QByteArray& data) = 0;
    virtual void FillInternalBuffer(QByteArray& data) = 0;
    virtual void DispatchPackage(QString& self, char* data, quint32& dataSize) = 0;
    virtual bool DataRecived(QDataStream& stream, quint32 len, QString self) = 0;
public:
    QByteArray m_Package;

Q_SIGNALS:
    void recivedDataForMe(QByteArray& package, QByteArray& data);
    void recivedDataForOther(QString& dst, QByteArray& package, QByteArray& data);
    void recivedDataForAll(QString& src, QByteArray& package, QByteArray& data);
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // IPROTOCOL_H
