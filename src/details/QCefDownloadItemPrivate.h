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

  quint32 id = 0;

  bool isInProgress = 0;
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

  CefRefPtr<CefBeforeDownloadCallback> downloadCallback;
  CefRefPtr<CefDownloadItemCallback> itemCallback;

public:
  static QSharedPointer<QCefDownloadItem> createQCefDownloadItem(CCefClientDelegate::RefPtr handerl,
                                                                 CefDownloadItem& cefItem,
                                                                 CefRefPtr<CefBeforeDownloadCallback>& callback);

  static void updateDownloadItem(QCefDownloadItem* p,
                                 CefDownloadItem& cefItem,
                                 CefRefPtr<CefDownloadItemCallback>& requestCallback);

  static void destroyQCefDownloadItem(QCefDownloadItem* p);
};
