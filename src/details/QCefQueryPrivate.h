#pragma once

#pragma region qt_headers
#include <QString>
#pragma endregion 

#include <QPointer>

#include "QCefViewPrivate.h"

class QCefQueryPrivate
{
public:
  QCefQueryPrivate();

  ~QCefQueryPrivate();

  int64_t id_ = -1;
  QString request_;
  QString response_;
  bool restult_ = true;
  int error_ = 0;

  bool replyed_ = false;
  QPointer<QCefViewPrivate> source_;
};
