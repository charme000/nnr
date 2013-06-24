#ifndef PROVIDERFACTORY_H
#define PROVIDERFACTORY_H

#include "Common.h"
#include "IProvider.h"
#include "TcpProvider.h"
#include "PipeProvider.h"
#include "UserUnit.h"
#include "Bus.h"
#include "nnr_global.h"

QT_BEGIN_NAMESPACE
class IProvider;
class Bus;
QT_END_NAMESPACE

/*! \class ProviderFactory
    \brief 传输体工厂类
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

class NNR_EXPORT ProviderFactory : public QObject
{
        Q_OBJECT
    public:
        static IProvider* Create(Bus* b, void* v, Flag_Tag ft)
        {
            QPointer<IProvider> provider = NULL;

            switch (ft)
            {
                case TcpFlag:
                    provider = new TcpProvider((QTcpSocket*)v);
                    provider->m_bus = b;
                    break;
                case PipeFlag:
                    provider = new PipeProvider((QLocalSocket*)v);
                    provider->m_bus = b;
                    break;
                default:
                    break;
            }

            return provider;
        }
};

#endif // PROVIDERFACTORY_H
