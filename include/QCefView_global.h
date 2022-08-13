/*
 * File: QCefView_global.h
 * Project: QCefView
 * Created: 29th March 2016
 * Author: Sheen
 * Source: https://github.com/cefview/qcefview
 * Docs: https://cefview.github.io/QCefView/
 */
#ifndef QCEFEVIEW_GLOBAL_H
#define QCEFEVIEW_GLOBAL_H
#pragma once

#pragma region qt_headers
#include <QtCore/qglobal.h>

#include <QMetaType>
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
