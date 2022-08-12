#include "CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>

#include "QCefDownloadItemPrivate.h"
#include "QCefViewPrivate.h"
#include "utils/CommonUtils.h"
#include "utils/ValueConvertor.h"

void
CCefClientDelegate::insertDownloadItem(QCefDownloadItemPointer item)
{
  downloadItemMap_[item->id()] = item;
}

void
CCefClientDelegate::removeDownloadItem(QCefDownloadItemPointer item)
{
  downloadItemMap_.remove(item->id());
}

void
CCefClientDelegate::onBeforeDownload(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefDownloadItem> download_item,
                                     const CefString& suggested_name,
                                     CefRefPtr<CefBeforeDownloadCallback> callback)
{
  FLog();

  if (!IsValidBrowser(browser))
    return;

  auto item = QCefDownloadItemPrivate::createQCefDownloadItem(shared_from_this(),
                                                              *(download_item.get()), //
                                                              callback);

  pCefViewPrivate_->q_ptr->newDownloadItem(item);
}

void
CCefClientDelegate::onDownloadUpdated(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDownloadItem> download_item,
                                      CefRefPtr<CefDownloadItemCallback> callback)
{
  FLog();

  if (!IsValidBrowser(browser))
    return;

  if (!download_item->IsValid())
    return;

  auto it = downloadItemMap_.find(download_item->GetId());
  if (it == downloadItemMap_.end())
    return;

  auto item = it.value();
  QCefDownloadItemPrivate::updateDownloadItem(item.data(),            //
                                              *(download_item.get()), //
                                              callback);

  if (item->isCanceled())
    removeDownloadItem(item);

  pCefViewPrivate_->q_ptr->updateDownloadItem(item);
}
