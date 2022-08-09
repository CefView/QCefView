#include <QCefDownloadItem.h>

#include <QSharedPointer>

#include "details/QCefDownloadItemPrivate.h"
#include "details/utils/CommonUtils.h"

REGISTER_METATYPE(QCefDownloadItem);

QCefDownloadItem::QCefDownloadItem()
  : d_ptr(new QCefDownloadItemPrivate())
{}

QCefDownloadItem::~QCefDownloadItem() {}

void
QCefDownloadItem::start(const QString& path, bool useDefaultDialog)
{
  Q_D(QCefDownloadItem);
  d_ptr->downloadCallback->Continue(path.toStdString(), useDefaultDialog);

  auto handler = d_ptr->handler.lock();
  if (!handler)
    return;

  handler->insertDownloadItem(sharedFromThis());
}

void
QCefDownloadItem::pause() const
{
  Q_D(const QCefDownloadItem);
  d_ptr->itemCallback->Pause();
}

void
QCefDownloadItem::resume() const
{
  Q_D(const QCefDownloadItem);
  d_ptr->itemCallback->Resume();
}

void
QCefDownloadItem::cancel() const
{
  Q_D(const QCefDownloadItem);
  d_ptr->itemCallback->Cancel();
}

bool
QCefDownloadItem::isInProgress() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->isInProgress;
}

bool
QCefDownloadItem::isComplete() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->isComplete;
}

bool
QCefDownloadItem::isCanceled() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->isCanceled;
}

qint64
QCefDownloadItem::currentSpeed() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->currentSpeed;
}

int
QCefDownloadItem::percentComplete() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->isCanceled;
}

qint64
QCefDownloadItem::totalBytes() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->isCanceled;
}

qint64
QCefDownloadItem::receivedBytes() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->isCanceled;
}

QDateTime
QCefDownloadItem::startTime() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->startTime;
}

QDateTime
QCefDownloadItem::endTime() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->endTime;
}

QString
QCefDownloadItem::fullPath() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->fullPath;
}

quint32
QCefDownloadItem::id() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->id;
}

QString
QCefDownloadItem::url() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->url;
}

QString
QCefDownloadItem::originalUrl() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->originalUrl;
}

QString
QCefDownloadItem::suggestedFileName() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->suggestedFileName;
}

QString
QCefDownloadItem::contentDisposition() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->contentDisposition;
}

QString
QCefDownloadItem::mimeType() const
{
  Q_D(const QCefDownloadItem);
  return d_ptr->mimeType;
}
