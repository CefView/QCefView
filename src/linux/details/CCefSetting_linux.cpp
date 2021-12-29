#include "details/CCefSetting.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

CCefSetting::CCefSetting()
{
  userAgent_.FromString(CEFVIEW_USER_AGENT);

  QDir ExeDir = QCoreApplication::applicationDirPath();

  QString strExePath = ExeDir.filePath(RENDER_PROCESS_NAME);
  browserSubProcessPath_.FromString(QDir::toNativeSeparators(strExePath).toStdString());

  QString strResPath = ExeDir.filePath(RESOURCE_DIRECTORY_NAME);
  resourceDirectoryPath_.FromString(QDir::toNativeSeparators(strResPath).toStdString());

  QDir ResPath(strResPath);
  localesDirectoryPath_.FromString(QDir::toNativeSeparators(ResPath.filePath(LOCALES_DIRECTORY_NAME)).toStdString());
}
