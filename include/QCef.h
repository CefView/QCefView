#ifndef QCEF_H
#define QCEF_H
#pragma once
#include <QCefView_global.h>
#include <QCefSetting.h>

#pragma region qt_headers
#pragma endregion qt_headers

/// <summary>
///
/// </summary>
namespace QCef {
bool QCEFVIEW_EXPORT
Initialize(const QCefSetting& settings, int argc, char* argv[]);

void QCEFVIEW_EXPORT
Uninitialize();
}; // namespace QCef

#endif // QCEF_H
