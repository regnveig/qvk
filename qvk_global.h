#ifndef QVK_GLOBAL_H
#define QVK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined QVK_LIBRARY
    #define QVK_EXPORT Q_DECL_EXPORT
#else
    #define QVK_EXPORT Q_DECL_IMPORT
#endif

#endif // QVK_GLOBAL_H
