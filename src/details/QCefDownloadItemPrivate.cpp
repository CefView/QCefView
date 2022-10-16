#include "QCefDownloadItemPrivate.h"

#include <QCefDownloadItem.h>

QSharedPointer<QCefDownloadItem>
QCefDownloadItemPrivate::createQCefDownloadItem(CCefClientDelegate::RefPtr handler,
                                                CefDownloadItem& cefItem,
                                                CefRefPtr<CefBeforeDownloadCallback>& callback)
{
  auto p = new QCefDownloadItem();
  p->d_ptr->handler = handler;

  p->d_ptr->id = cefItem.GetId();

  p->d_ptr->isInProgress = cefItem.IsInProgress();
  p->d_ptr->isComplete = cefItem.IsComplete();
  p->d_ptr->isCanceled = cefItem.IsCanceled();

  p->d_ptr->percentComplete = cefItem.GetPercentComplete();
  p->d_ptr->currentSpeed = cefItem.GetCurrentSpeed();
  p->d_ptr->totalBytes = cefItem.GetTotalBytes();
  p->d_ptr->receivedBytes = cefItem.GetReceivedBytes();

  double t = 0;
  cef_time_t ct;

  cef_time_from_basetime(cefItem.GetStartTime(), &ct);

  if (cef_time_to_doublet(&ct, &t))
    p->d_ptr->startTime = QDateTime::fromSecsSinceEpoch(t);

  cef_time_from_basetime(cefItem.GetEndTime(), &ct);
  if (cef_time_to_doublet(&ct, &t))
    p->d_ptr->endTime = QDateTime::fromSecsSinceEpoch(t);

  p->d_ptr->fullPath = QString::fromStdString(cefItem.GetFullPath().ToString());
  p->d_ptr->url = QString::fromStdString(cefItem.GetURL().ToString());
  p->d_ptr->originalUrl = QString::fromStdString(cefItem.GetOriginalUrl().ToString());
  p->d_ptr->suggestedFileName = QString::fromStdString(cefItem.GetSuggestedFileName().ToString());
  p->d_ptr->contentDisposition = QString::fromStdString(cefItem.GetContentDisposition().ToString());
  p->d_ptr->mimeType = QString::fromStdString(cefItem.GetMimeType().ToString());

  p->d_ptr->downloadCallback = callback;
  return QSharedPointer<QCefDownloadItem>(p, &QCefDownloadItemPrivate::destroyQCefDownloadItem);
}

void
QCefDownloadItemPrivate::updateDownloadItem(QCefDownloadItem* p,
                                            CefDownloadItem& cefItem,
                                            CefRefPtr<CefDownloadItemCallback>& callback)
{
  p->d_ptr->id = cefItem.GetId();

  p->d_ptr->isInProgress = cefItem.IsInProgress();
  p->d_ptr->isComplete = cefItem.IsComplete();
  p->d_ptr->isCanceled = cefItem.IsCanceled();

  p->d_ptr->percentComplete = cefItem.GetPercentComplete();
  p->d_ptr->currentSpeed = cefItem.GetCurrentSpeed();
  p->d_ptr->totalBytes = cefItem.GetTotalBytes();
  p->d_ptr->receivedBytes = cefItem.GetReceivedBytes();

  double t = 0;
  cef_time_t ct;

  cef_time_from_basetime(cefItem.GetStartTime(), &ct);
  if (cef_time_to_doublet(&ct, &t))
    p->d_ptr->startTime = QDateTime::fromSecsSinceEpoch(t);

  cef_time_from_basetime(cefItem.GetEndTime(), &ct);
  if (cef_time_to_doublet(&ct, &t))
    p->d_ptr->endTime = QDateTime::fromSecsSinceEpoch(t);

  p->d_ptr->fullPath = QString::fromStdString(cefItem.GetFullPath().ToString());
  p->d_ptr->url = QString::fromStdString(cefItem.GetURL().ToString());
  p->d_ptr->originalUrl = QString::fromStdString(cefItem.GetOriginalUrl().ToString());
  p->d_ptr->suggestedFileName = QString::fromStdString(cefItem.GetSuggestedFileName().ToString());
  p->d_ptr->contentDisposition = QString::fromStdString(cefItem.GetContentDisposition().ToString());
  p->d_ptr->mimeType = QString::fromStdString(cefItem.GetMimeType().ToString());

  p->d_ptr->itemCallback = callback;
}

void
QCefDownloadItemPrivate::destroyQCefDownloadItem(QCefDownloadItem* p)
{
  if (!p)
    return;

  if (p->d_ptr && p->d_ptr->itemCallback)
    p->d_ptr->itemCallback->Cancel();

  delete p;
}
