#include "Bus_p.h"

/*! \class BusPrivate
    \brief 内部类
    \version 1.0
    \date 2012.12.1-2012.1.30
  */

QT_BEGIN_NAMESPACE

BusPrivate::BusPrivate(Bus* b): q_ptr(b)
{
}

void BusPrivate::Send_p(QByteArray& data)
{
    if(m_p->AnalysisPackage(data) == true)
    {
        QList<UserUnit*> list = userMap.values();

        for(int i = 0; i < list.size(); ++i)
        {
            QByteArray package = m_p->GetPackage();
            list.value(i)->SendPackage(package);
        }

        m_p->GetPackage().clear();
    }
}

void BusPrivate::SendPackageToAll_p(QString& srcUuid, QByteArray& package, QByteArray& data)
{
    if(data.isEmpty() == false)
    {
        QByteArray byte;
        QDataStream tmp(&byte, QIODevice::WriteOnly);
        tmp << data;
        package.append(byte);
    }

    QList<UserUnit*> list = userMap.values();
    QList<QString> uuidList = userMap.keys();
    int index = uuidList.indexOf(srcUuid.toUpper());

    if(index != -1)
    {
        for(int i = 0; i < list.size(); ++i)
        {
            if(i != index)
            {
                list.value(i)->SendPackage(package);
            }
        }
    }
}

void BusPrivate::SendPackageToOne_p(QString& dstUuid, QByteArray& package, QByteArray& data)
{
    if(data.isEmpty() == false)
    {
        QByteArray byte;
        QDataStream tmp(&byte, QIODevice::WriteOnly);
        tmp << data;
        package.append(byte);
    }

    QList<QString> uuidList = userMap.keys();
    int index = uuidList.indexOf(dstUuid.toUpper());

    if(index != -1)
    {
        QList<UserUnit*> userList = userMap.values();
        userList.value(index)->SendPackage(package);
    }
}

bool BusPrivate::Add_p(UserUnit& user, QString& guid)
{
    bool ret = false;
    if(!userMap.contains(guid))
    {
        userMap.insert(guid, &user);
        ret = true;
    }
    return ret;
}

void BusPrivate::DeleteAll_p(Flag_Tag flag,bool immediate)
{
    if(flag != InvalidFlag)
    {
        QMapIterator<QString, UserUnit*> i(userMap);

        while(i.hasNext())
        {
            i.next();
            CloseConnect(i.value()->m_f,flag,immediate);
        }
    }

    userMap.clear();
}

bool BusPrivate::Delete_p(QString& uuid,Flag_Tag flag, bool immediate)
{
    bool ret = false;

    if(flag != InvalidFlag)
    {
        CloseConnect(userMap.value(uuid)->m_f,flag,immediate);
    }

    if(userMap.contains(uuid))
    {
        if(userMap.value(uuid) != NULL)
        {
            delete userMap.value(uuid);
        }

        if(1 == userMap.remove(uuid))
        {
            ret = true;
        }
    }

    return ret;
}

bool BusPrivate::Delete_p(UserUnit& user, Flag_Tag flag, bool immediate)
{
    bool ret = false;
    QString key = userMap.key(&user);

    if(flag != InvalidFlag)
    {
        CloseConnect(user.m_f,flag,immediate);
    }

    if(userMap.contains(key))
    {
        if(&user != NULL)
        {
            delete &user;
        }

        if(1 == userMap.remove(key))
        {
            ret = true;
        }
    }

    return ret;
}

UserUnit* BusPrivate::GetUser_p(QString& uuid)
{
    return userMap.value(uuid);
}

QString BusPrivate::GetGuid_p(UserUnit& user)
{
    return userMap.key(&user);
}

QString BusPrivate::GetGuid_p(void* value, Flag_Tag flag)
{
    QString guid;
    QList<UserUnit*> list = userMap.values();
    QList<QString> list1 = userMap.keys();

    for(int i = 0; i < list.size(); ++i)
    {
        if(flag == TcpFlag)
        {
            if((void*)(list.at(i)->m_f) == value)
            {
                guid = list1.takeAt(i);
            }
        }
        else if(flag == PipeFlag)
        {
            if((void*)(list.at(i)->m_f) == value)
            {
                guid = list1.takeAt(i);
            }
        }
    }

    return guid;
}

UserUnit* BusPrivate::GetUser_p(void* flag)
{
    UserUnit* user = NULL;
    QList<UserUnit*> list = userMap.values();

    for(int i = 0; i < list.size(); ++i)
    {
        if((void*)(list.at(i)->m_f) == flag)
        {
            user = list.takeAt(i);
        }
    }

    return user;
}

void BusPrivate::CloseConnect(void* value, Flag_Tag flag, bool immediate)
{
    if(value != NULL)
    {
        if(flag == TcpFlag)
        {
            QTcpSocket* t = (QTcpSocket*)value;

            if(t->state() != QAbstractSocket::UnconnectedState)
            {
                if(immediate == true)
                {
                    //立刻关闭
                    t->abort();
                }
                else
                {
                    //这种关闭如果发现有未接完的数据的话会等待
                    t->disconnectFromHost();
                }
            }
        }
        else if(flag == PipeFlag)
        {
            QLocalSocket* t = (QLocalSocket*)value;

            if(t->state() != QLocalSocket::UnconnectedState)
            {
                if(immediate == true)
                {
                    //立刻关闭
                    t->abort();
                }
                else
                {
                    //这种关闭如果发现有未接完的数据的话会等待
                    t->disconnectFromServer();
                }
            }
        }
    }
}

QT_END_NAMESPACE
