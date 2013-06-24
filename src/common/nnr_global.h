#ifndef NNR_GLOBAL_H
#define NNR_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef NNR_LIB
# define NNR_EXPORT Q_DECL_EXPORT
#else
# define NNR_EXPORT Q_DECL_IMPORT
#endif

#endif // NNR_GLOBAL_H
