#ifndef MANAGERFACTORY_H
#define MANAGERFACTORY_H

#include "Common.h"
#include "Manager.h"
#include "TcpManager.h"
#include "PipeManager.h"
#include "Bus.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE
class Bus;
class Manager;

/*! \class ManagerFactory
    \brief 管理器工厂类
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

class NNR_EXPORT ManagerFactory
{
    public:
        static Manager* Create(Bus& bus, QString &attribute, Flag_Tag method, QString &value, QString &ip, QString &guidName)
        {
            QPointer<Manager> m;

            if (method == TcpFlag)
            {
                m = new TcpManager(bus, attribute, value, ip, guidName);
            }
            else if (method == PipeFlag)
            {
                m = new PipeManager(bus,attribute,value, ip, guidName);
            }
            else
            {
            }

            return m;
        }
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // MANAGERFACTORY_H
