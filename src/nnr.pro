######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 23 13:32:33 2012
######################################################################

TEMPLATE = lib
TARGET =
DEPENDPATH += . common nnr nnr_private
INCLUDEPATH += . common nnr nnr_private

QT += network
DEFINES += NNR_LIB

# Input
HEADERS += common/Common.h \
           common/nnr_global.h \
           nnr/Base.h \
           nnr/Bus.h \
           nnr/Defines.h \
           nnr/Encoder.h \
           nnr/IProtocol.h \
           nnr/IProvider.h \
           nnr/Macro.h \
           nnr/Manager.h \
           nnr/ManagerFactory.h \
           nnr/PackageProtocol_1_0.h \
           nnr/PipeManager.h \
           nnr/PipeProvider.h \
           nnr/ProtocolDefines.h \
           nnr/ProtocolFactory.h \
           nnr/ProviderFactory.h \
           nnr/TcpManager.h \
           nnr/TcpProvider.h \
           nnr/UserUnit.h \
           nnr_private/Base_p.h \
           nnr_private/Bus_p.h \
           nnr_private/UserUnit_p.h
SOURCES += nnr/Base.cpp \
           nnr/Bus.cpp \
           nnr/Encoder.cpp \
           nnr/Manager.cpp \
           nnr/PackageProtocol_1_0.cpp \
           nnr/PipeManager.cpp \
           nnr/PipeProvider.cpp \
           nnr/ProtocolFactory.cpp \
           nnr/TcpManager.cpp \
           nnr/TcpProvider.cpp \
           nnr/UserUnit.cpp \
           nnr_private/BasePrivate.cpp \
           nnr_private/BusPrivate.cpp \
           nnr_private/UserUnitPrivate.cpp
