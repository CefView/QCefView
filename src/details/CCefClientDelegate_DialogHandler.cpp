﻿#include "CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "QCefViewPrivate.h"
#include "utils/CommonUtils.h"
#include "utils/ValueConvertor.h"

bool
CCefClientDelegate::onFileDialog(CefRefPtr<CefBrowser> browser,
                                 CefBrowserHost::FileDialogMode mode,
                                 const CefString& title,
                                 const CefString& default_file_path,
                                 const std::vector<CefString>& accept_filters,
                                 CefRefPtr<CefFileDialogCallback> callback)
{
#if defined (Q_OS_LINUX)
  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
    pCefViewPrivate_->onFileDialog(browser, mode, title, default_file_path, accept_filters, callback);
  });
  return true;
#endif
  return false;
}
