#ifndef MACRO_H
#define MACRO_H

#define FLAG_MACRO(s,f) \
    FlagStruct ff;\
    ff.s##Flag = f;\
    QVariant v; \
    v.setValue(ff);

#define START_MACRO(s,f) \
    StartStruct ff;\
    ff.s = f;\
    QVariant v; \
    v.setValue(ff);

#define VFlag v

#define BIND(a,b) a##b

#define SINGELTON(classname) \
    public:\
    static classname* GetInstance()\
    {\
        if(classname::m_pIns == NULL)\
        {\
            classname::m_pIns = new classname();\
        }\
        return classname::m_pIns;\
    }\
    private:\
    static classname* classname::m_pIns;\
    class Garbo\
    {\
    public:\
        ~Garbo()\
        {\
            if(classname::m_pIns)\
            {\
                delete classname::m_pIns;\
            }\
        }\
    };\
    static Garbo gb;

#define EMPTY ""

#define USING_PROTOCOL(b,v); \
    PackageProtocol_##v * p = qobject_cast<PackageProtocol_##v*>(b->getProtocol());

#define PACKAGE_PROTOCOL(b,v); \
    PackageProtocol_##v* p = qobject_cast<PackageProtocol_##v*>(ProtocolFactory::Create(b));

#endif // MACRO_H
