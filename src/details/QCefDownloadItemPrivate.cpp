#include "QCefDownloadItemPrivate.h"

#pragma region cef_headers
#include <include/cef_version.h>
#pragma endregion 

QSharedPointer<QCefDownloadItem>
QCefDownloadItemPrivate::create(CCefClientDelegate::RefPtr handler)
{
  return QSharedPointer<QCefDownloadItem>(new QCefDownloadItem(), &QCefDownloadItemPrivate::destroy);
}

void
QCefDownloadItemPrivate::setSuggestedName(QCefDownloadItem* item, const QString& suggestedFileName)
{
  auto p = item->d_ptr.data();
  p->suggestedFileName = suggestedFileName;
}

void
QCefDownloadItemPrivate::update(QCefDownloadItem* item, CefDownloadItem& cefItem)
{
  auto p = item->d_ptr.data();
  p->id = cefItem.GetId();

  p->isInProgress = cefItem.IsInProgress();
  p->isComplete = cefItem.IsComplete();
  p->isCanceled = cefItem.IsCanceled();

  p->percentComplete = cefItem.GetPercentComplete();
  p->currentSpeed = cefItem.GetCurrentSpeed();
  p->totalBytes = cefItem.GetTotalBytes();
  p->receivedBytes = cefItem.GetReceivedBytes();

  double t = 0;
  cef_time_t ct;

#if (CEF_VERSION_MAJOR) > 104 || (CEF_VERSION_MAJOR == 104 && CEF_VERSION_MINOR > 4)
  cef_time_from_basetime(cefItem.GetStartTime(), &ct);
#else
  ct = cefItem.GetStartTime();
#endif
  if (cef_time_to_doublet(&ct, &t))
    p->startTime = QDateTime::fromSecsSinceEpoch(t);

#if (CEF_VERSION_MAJOR) > 104 || (CEF_VERSION_MAJOR == 104 && CEF_VERSION_MINOR > 4)
  cef_time_from_basetime(cefItem.GetEndTime(), &ct);
#else
  ct = cefItem.GetEndTime();
#endif
  if (cef_time_to_doublet(&ct, &t))
    p->endTime = QDateTime::fromSecsSinceEpoch(t);

  p->fullPath = QString::fromStdString(cefItem.GetFullPath().ToString());
  p->url = QString::fromStdString(cefItem.GetURL().ToString());
  p->originalUrl = QString::fromStdString(cefItem.GetOriginalUrl().ToString());
  p->mimeType = QString::fromStdString(cefItem.GetMimeType().ToString());

  // update suggested name if not empty
  auto suggestedFileName = QString::fromStdString(cefItem.GetSuggestedFileName().ToString());
  if (!suggestedFileName.isEmpty()) {
    p->suggestedFileName = suggestedFileName;
  }

  // update content disposition if not empty
  auto contentDisposition = QString::fromStdString(cefItem.GetContentDisposition().ToString());
  if (!contentDisposition.isEmpty()) {
    p->contentDisposition = contentDisposition;
  }
}

void
QCefDownloadItemPrivate::setBeforeDownloadCallback(QCefDownloadItem* item,
                                                   CefRefPtr<CefBeforeDownloadCallback> beforeDownloadCallback)
{
  auto p = item->d_ptr.data();
  p->beforeDownloadCallback = beforeDownloadCallback;
}

void
QCefDownloadItemPrivate::setDownloadItemCallback(QCefDownloadItem* item,
                                                 CefRefPtr<CefDownloadItemCallback> downloadItemCallback)
{
  auto p = item->d_ptr.data();
  p->downloadItemCallback = downloadItemCallback;
}

void
QCefDownloadItemPrivate::destroy(QCefDownloadItem* item)
{
  if (!item)
    return;

  auto p = item->d_ptr.data();
  if (p && p->downloadItemCallback)
    p->downloadItemCallback->Cancel();

  delete item;
}
