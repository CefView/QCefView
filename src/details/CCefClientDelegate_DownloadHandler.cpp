#include "CCefClientDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"


void
CCefClientDelegate::onBeforeDownload(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefDownloadItem> download_item,
                              const CefString& suggested_name,
                              CefRefPtr<CefBeforeDownloadCallback> callback)
{
    if (pCefViewPrivate_)
    {
      return pCefViewPrivate_->onBeforeDownload(browser, download_item, suggested_name, callback);
    }
}

void
CCefClientDelegate::onDownloadUpdated(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefDownloadItem> download_item,
                               CefRefPtr<CefDownloadItemCallback> callback)
{
    if (pCefViewPrivate_)
    {
      return pCefViewPrivate_->onDownloadUpdated(browser, download_item, callback);
    }
}
