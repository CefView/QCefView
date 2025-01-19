#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "details/QCefViewPrivate.h"
#include "details/utils/CommonUtils.h"
#include "details/utils/ValueConvertor.h"

bool
CCefClientDelegate::onFileDialog(CefRefPtr<CefBrowser>& browser,
                                 CefBrowserHost::FileDialogMode mode,
                                 const CefString& title,
                                 const CefString& default_file_path,
                                 const std::vector<CefString>& accept_filters,
#if CEF_VERSION_MAJOR < 102
                                 int selected_accept_filter,
#endif
                                 CefRefPtr<CefFileDialogCallback>& callback)
{
  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
    QStringList filters;
    if (!accept_filters.empty()) {
      for (const auto& filter : accept_filters) {
        filters << "*" + QString::fromStdString(filter.ToString());
      }
    }
    pCefViewPrivate_->onFileDialog(mode,
                                   title.ToString().c_str(),
                                   default_file_path.ToString().c_str(),
                                   filters,
#if CEF_VERSION_MAJOR < 102
                                   selected_accept_filter,
#endif
                                   callback);
  });
  return true;
}
