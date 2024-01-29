#include "QCefQueryPrivate.h"

QCefQueryPrivate::QCefQueryPrivate() {}

QCefQueryPrivate::~QCefQueryPrivate()
{
  if (!replyed_ && source_) {
    replyed_ = true;
    source_->responseQCefQuery(id_, restult_, response_, error_);
  }
}
