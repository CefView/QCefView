#include "QCefSettingPrivate.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#include <QString>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

QCefSettingPrivate::QCefSettingPrivate() {}

void
QCefSettingPrivate::CopyToCefBrowserSettings(CefBrowserSettings& settings) const
{}
