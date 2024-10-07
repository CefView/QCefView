#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "details/QCefViewPrivate.h"
#include "details/utils/CommonUtils.h"
#include "details/utils/ValueConvertor.h"

bool
CCefClientDelegate::onFileDialog(CefRefPtr<CefBrowser> browser,
                                 CefBrowserHost::FileDialogMode mode,
                                 const CefString& title,
                                 const CefString& default_file_path,
                                 const std::vector<CefString>& accept_filters,
#if CEF_VERSION_MAJOR < 102
                                 int selected_accept_filter,
#endif
                                 CefRefPtr<CefFileDialogCallback> callback)
{
#if defined(Q_OS_LINUX)
  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
    pCefViewPrivate_->onFileDialog(mode,
                                   title,
                                   default_file_path,
                                   accept_filters,
#if CEF_VERSION_MAJOR < 102
                                   selected_accept_filter,
#endif
                                   callback);
  });
  return true;
#endif
  return false;
}
