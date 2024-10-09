#pragma once

#pragma region qt_headers
#include <QString>
#include <QVariantMap>
#pragma endregion 

class QCefEventPrivate
{
public:
  QCefEventPrivate();

  ~QCefEventPrivate();

  QString name;
  QVariantList args;
};
