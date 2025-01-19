#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "details/QCefDownloadItemPrivate.h"
#include "details/QCefViewPrivate.h"
#include "details/utils/CommonUtils.h"
#include "details/utils/ValueConvertor.h"

void
CCefClientDelegate::onBeforeDownload(CefRefPtr<CefBrowser>& browser,
                                     CefRefPtr<CefDownloadItem>& download_item,
                                     const CefString& suggested_name,
                                     CefRefPtr<CefBeforeDownloadCallback>& callback)
{
  FLog();

  // qDebug() << "onBeforeDownload, percent: " << download_item->GetPercentComplete() << "% \n"
  //          << download_item->GetTotalBytes() << "/" << download_item->GetReceivedBytes() << "\n"
  //          << "  inProgress: " << download_item->IsInProgress() << "\n"
  //          << "  canceled: " << download_item->IsCanceled() << "\n"
  //          << "  complete: " << download_item->IsComplete();

  if (!IsValidBrowser(browser))
    return;

  // get id
  auto id = download_item->GetId();

  // process item
  QWeakPointer<QCefDownloadItem> weakRefItem;
  {
    QSharedPointer<QCefDownloadItem> item;
    auto it = pendingDownloadItemMap_.find(id);

    // find
    if (it != pendingDownloadItemMap_.end()) {
      // found, obtain item and remove it from pending map
      item = it.value();
      pendingDownloadItemMap_.erase(it);
    }

    // validate
    if (!item) {
      // pending item not found or invalid, create new pending item
      item = QCefDownloadItemPrivate::create(shared_from_this());
    }

    // set suggested file name
    auto suggestedFileName = QString::fromStdString(suggested_name);
    QCefDownloadItemPrivate::setSuggestedName(item.data(), suggestedFileName);

    // update
    QCefDownloadItemPrivate::update(item.data(), *(download_item.get()));
    QCefDownloadItemPrivate::setBeforeDownloadCallback(item.data(), callback);

    // notify user of the new download item
    weakRefItem = item;

    // marshal to main UI thread and need to block
    Qt::ConnectionType c = pCefViewPrivate_->q_ptr->thread() == QThread::currentThread() ? Qt::DirectConnection
                                                                                         : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(
      pCefViewPrivate_, [=]() { pCefViewPrivate_->onNewDownloadItem(item, suggestedFileName); }, c);

    item.reset();
  }

  // check whether user kept the item or not
  auto item = weakRefItem.lock();
  if (item) {
    // user kept the item, keep it in confirmed map
    confirmedDownloadItemMap_[id] = item;
  } else {
    // user discarded this, place a null item into the pending map
    pendingDownloadItemMap_[id] = nullptr;
  }
}

void
CCefClientDelegate::onDownloadUpdated(CefRefPtr<CefBrowser>& browser,
                                      CefRefPtr<CefDownloadItem>& download_item,
                                      CefRefPtr<CefDownloadItemCallback>& callback)
{
  FLog();

  // qDebug() << "onDownloadUpdated, percent: " << download_item->GetPercentComplete() << "% \n"
  //          << download_item->GetTotalBytes() << "/" << download_item->GetReceivedBytes() << "\n"
  //          << "  inProgress: " << download_item->IsInProgress() << "\n"
  //          << "  canceled: " << download_item->IsCanceled() << "\n"
  //          << "  complete: " << download_item->IsComplete();

  if (!IsValidBrowser(browser))
    return;

  if (!download_item->IsValid())
    return;

  // get id
  auto id = download_item->GetId();

  // find in confirmed map
  {
    auto it = confirmedDownloadItemMap_.find(id);
    if (it != confirmedDownloadItemMap_.end()) {
      // confirmed item found
      auto item = it.value().lock();
      if (!item) {
        // TODO(sheen): something went wrong, but i haven't got a good solution
        confirmedDownloadItemMap_.erase(it);
        return;
      }

      if (!item->isStarted()) {
        // not started by user yet, pause it
        callback->Pause();
        return;
      }

      // update
      QCefDownloadItemPrivate::update(item.data(), *(download_item.get()));
      QCefDownloadItemPrivate::setDownloadItemCallback(item.data(), callback);

      // notify (marshal to main UI thread but no need to block)
      QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onUpdateDownloadItem(item); });

      // check status
      if (download_item->IsCanceled() || download_item->IsComplete()) {
        confirmedDownloadItemMap_.remove(id);
        return;
      }

      return;
    }
  }

  // check status
  if (download_item->IsCanceled() || download_item->IsComplete()) {
    pendingDownloadItemMap_.remove(id);
    return;
  }

  // find in pending map
  {
    QSharedPointer<QCefDownloadItem> item;
    auto it = pendingDownloadItemMap_.find(id);

    // find
    if (it == pendingDownloadItemMap_.end()) {
      // not found, this is new download item
      item = QCefDownloadItemPrivate::create(shared_from_this());
      pendingDownloadItemMap_[id] = item;

      // pause it
      callback->Pause();
    } else {
      // found,
      item = it.value();

      // validate
      if (!item) {
        // null item, it means this download item has been discarded by the user
        return;
      }
    }

    // update pending item
    QCefDownloadItemPrivate::update(item.data(), *(download_item.get()));
    QCefDownloadItemPrivate::setDownloadItemCallback(item.data(), callback);
  }
}
