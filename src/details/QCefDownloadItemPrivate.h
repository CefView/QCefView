#pragma once

#include <QDateTime>
#include <QSharedPointer>
#include <QString>
#include <QWeakPointer>

#include <include/cef_app.h>

#include "CCefClientDelegate.h"

class QCefDownloadItem;

class QCefDownloadItemPrivate
{
public:
  CCefClientDelegate::WeakPtr handler;
  bool isStarted = false;

  quint32 id = 0;

  bool isInProgress = false;
  bool isComplete = false;
  bool isCanceled = false;

  int percentComplete = 0;
  qint64 currentSpeed = 0;
  qint64 totalBytes = 0;
  qint64 receivedBytes = 0;

  QDateTime startTime;
  QDateTime endTime;

  QString fullPath;
  QString url;
  QString originalUrl;
  QString suggestedFileName;
  QString contentDisposition;
  QString mimeType;

  CefRefPtr<CefBeforeDownloadCallback> beforeDownloadCallback;
  CefRefPtr<CefDownloadItemCallback> downloadItemCallback;

public:
  static QSharedPointer<QCefDownloadItem> create(CCefClientDelegate::RefPtr handler);

  static void setSuggestedName(QCefDownloadItem* item, const QString& suggestedFileName);

  static void update(QCefDownloadItem* item, CefDownloadItem& cefItem);

  static void setBeforeDownloadCallback(QCefDownloadItem* item,
                                        CefRefPtr<CefBeforeDownloadCallback> beforeDownloadCallback);

  static void setDownloadItemCallback(QCefDownloadItem* item, CefRefPtr<CefDownloadItemCallback> downloadItemCallback);

  static void destroy(QCefDownloadItem* item);
};
