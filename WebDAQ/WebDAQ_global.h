#ifndef WEBDAQ_GLOBAL_H
#define WEBDAQ_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WEBDAQ_LIBRARY)
#  define WEBDAQ_EXPORT Q_DECL_EXPORT
#else
#  define WEBDAQ_EXPORT Q_DECL_IMPORT
#endif

#endif // WEBDAQ_GLOBAL_H
