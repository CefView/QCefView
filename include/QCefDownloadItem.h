#ifndef QCEFDOWNLOADITEM_H
#define QCEFDOWNLOADITEM_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QDateTime>
#include <QEnableSharedFromThis>
#include <QSharedPointer>
#include <QString>
#pragma endregion qt_headers

class QCefDownloadItemPrivate;

/// <summary>
/// Represents the download item
/// </summary>
class QCEFVIEW_EXPORT QCefDownloadItem : public QEnableSharedFromThis<QCefDownloadItem>
{
  Q_DISABLE_COPY_MOVE(QCefDownloadItem)
  Q_DECLARE_PRIVATE(QCefDownloadItem)
  QScopedPointer<QCefDownloadItemPrivate> d_ptr;

  friend class QCefView;

  /// <summary>
  /// Constructs a download item instance
  /// </summary>
  QCefDownloadItem();

public:
  /// <summary>
  /// Destructs the event instance
  /// </summary>
  ~QCefDownloadItem();

  /// <summary>
  ///
  /// </summary>
  void start(const QString& path, bool useDefaultDialog = true);

  /// <summary>
  ///
  /// </summary>
  void pause() const;

  /// <summary>
  ///
  /// </summary>
  void resume() const;

  /// <summary>
  ///
  /// </summary>
  void cancel() const;

  /// <summary>
  // Returns true if the download is in progress.
  /// </summary>
  /// <returns></returns>
  bool isInProgress() const;

  /// <summary>
  // Returns true if the download is complete.
  /// </summary>
  bool isComplete() const;

  /// <summary>
  // Returns true if the download has been canceled or interrupted.
  /// </summary>
  bool isCanceled() const;

  /// <summary>
  // Returns a simple speed estimate in bytes/s.
  /// </summary>
  qint64 currentSpeed() const;

  /// <summary>
  // Returns the rough percent complete or -1 if the receive total size is
  // unknown.
  /// </summary>
  int percentComplete() const;

  /// <summary>
  // Returns the total number of bytes.
  /// </summary>
  qint64 totalBytes() const;

  /// <summary>
  // Returns the number of received bytes.
  /// </summary>
  qint64 receivedBytes() const;

  /// <summary>
  // Returns the time that the download started.
  /// </summary>
  QDateTime startTime() const;

  /// <summary>
  // Returns the time that the download ended.
  /// </summary>
  QDateTime endTime() const;

  /// <summary>
  // Returns the full path to the downloaded or downloading file.
  /// </summary>
  QString fullPath() const;

  /// <summary>
  // Returns the unique identifier for this download.
  /// </summary>
  quint32 id() const;

  /// <summary>
  // Returns the URL.
  /// </summary>
  QString url() const;

  /// <summary>
  // Returns the original URL before any redirections.
  /// </summary>
  QString originalUrl() const;

  /// <summary>
  // Returns the suggested file name.
  /// </summary>
  QString suggestedFileName() const;

  /// <summary>
  // Returns the content disposition.
  /// </summary>
  QString contentDisposition() const;

  /// <summary>
  // Returns the mime type.
  /// </summary>
  QString mimeType() const;
};

Q_DECLARE_METATYPE(QCefDownloadItem);

typedef QSharedPointer<QCefDownloadItem> QCefDownloadItemPointer;

#endif // QCEFDOWNLOADITEM_H
