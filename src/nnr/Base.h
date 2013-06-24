#ifndef BASE_H
#define BASE_H

#include "Common.h"
#include "nnr_global.h"
#include "Defines.h"
#include "Macro.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class BasePrivate;
class Bus;

class NNR_EXPORT Base : public QObject
{
    Q_OBJECT
public:
    explicit Base(QObject* parent = 0);
    virtual ~Base();

public:
    void setAttribute(const QString& strAttribute);
    QString attribute();
    void setFlag(Flag_Tag flag);
    Flag_Tag flag();
    void setValue(const QString& strValue);
    QString v();
    void setUuidName(const QString& strName);
    QString uuid();
	void setIp(const QString& strIp);
	QString ip();
	void setUseUuid(bool bUse);
	bool useUuid();

    typedef const char* SlotFunc;
    void Set(SlotFunc ConnectServerSuccess, SlotFunc AcceptNewClient, SlotFunc ConnectError, SlotFunc ServerDisconnected, SlotFunc ClientDisconnected, SlotFunc AddToBusFailed, SlotFunc DataArrived, SlotFunc ByteWriten,SlotFunc PreCheckFinished);
    void CreateManager(Bus& bus);

    void Start();
    bool Stop(QString& uuid,bool immediate);
    void StopAll(bool immediate);

protected:
    Base(BasePrivate& d, QObject*);

private:
    BasePrivate* const d_ptr;
    //Q_DISABLE_COPY(Base)
    Q_DECLARE_PRIVATE(Base)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // BASE_H
