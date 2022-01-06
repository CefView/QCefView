#include "QCefConfigPrivate.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#include <QString>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

QCefConfigPrivate::QCefConfigPrivate()
{
  userAgent_ = CEFVIEW_USER_AGENT;

#if !defined(OS_MACOS)
  QDir ExeDir = QCoreApplication::applicationDirPath();

  QString strExePath = ExeDir.filePath(RENDER_PROCESS_NAME);
  browserSubProcessPath_ = QDir::toNativeSeparators(strExePath).toStdString();

  QString strResPath = ExeDir.filePath(RESOURCE_DIRECTORY_NAME);
  resourceDirectoryPath_ = QDir::toNativeSeparators(strResPath).toStdString();

  QDir ResPath(strResPath);
  localesDirectoryPath_ = QDir::toNativeSeparators(ResPath.filePath(LOCALES_DIRECTORY_NAME)).toStdString();
#endif
}
