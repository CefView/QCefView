#pragma once

#pragma region qt_headers
#include <QString>
#pragma endregion qt_headers

#include <QCefQuery.h>

class QCefQueryPrivate
{
public:
  QCefQueryPrivate();

  ~QCefQueryPrivate();

  int64_t id_ = -1;
  QString request_;
  QString response_;
  bool restult_ = false;
  int error_ = 0;
};
