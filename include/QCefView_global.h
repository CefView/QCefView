#ifndef QCEFEVIEW_GLOBAL_H
#define QCEFEVIEW_GLOBAL_H
#pragma once

#pragma region qt_headers
#include <QtCore/qglobal.h>
#pragma endregion qt_headers

#ifdef QCEFVIEW_STATIC
#define QCEFVIEW_EXPORT
#else
#ifdef QCEFVIEW_LIB
#define QCEFVIEW_EXPORT Q_DECL_EXPORT
#else
#define QCEFVIEW_EXPORT Q_DECL_IMPORT
#if _WIN32
#pragma comment(lib, "QCefView.lib")
#endif
#endif
#endif

#endif // QCEFEVIEW_GLOBAL_H
