#include "details/CCefSetting.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

CCefSetting::CCefSetting()
{
  user_agent.FromString(CEFVIEW_USER_AGENT);
  
  QDir currentDir = QDir::current();

  QString renderExecutablePath = currentDir.filePath(RENDER_PROCESS_NAME ".exe");
  browser_sub_process_path.FromString(QDir::toNativeSeparators(renderExecutablePath).toStdString());

  QString resourceFolderPath = currentDir.filePath(RESOURCE_DIRECTORY_NAME);
  resource_directory_path.FromString(QDir::toNativeSeparators(resourceFolderPath).toStdString());

  QString localeFolderPath = resourceFolderPath.filePath(LOCALES_DIRECTORY_NAME);
  locales_directory_path.FromString(QDir::toNativeSeparators(localeFolderPath).toStdString());
}
