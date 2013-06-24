#ifndef PROTOCOLFACTORY_H
#define PROTOCOLFACTORY_H

#include "Common.h"
#include "IProtocol.h"
#include "PackageProtocol_1_0.h"
#include "nnr_global.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class IProtocol;

class NNR_EXPORT ProtocolFactory
{
public:
    static IProtocol* Create(QString& protocolType);
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // PROTOCOLFACTORY_H
