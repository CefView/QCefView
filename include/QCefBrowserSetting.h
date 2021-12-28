#ifndef QCEFBROWSERSETTING_H
#define QCEFBROWSERSETTING_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QObject>
#include <QString>
#include <QColor>
#pragma endregion qt_headers

class QCEFVIEW_EXPORT QCefBrowserSetting : public QObject
{
  Q_OBJECT

public:
  QCefBrowserSetting();

  ~QCefBrowserSetting();

private:
}; // namespace QCefBrowserSetting

#endif
