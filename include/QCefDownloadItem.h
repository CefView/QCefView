/*
 * File: QCefDownloadItem.h
 * Project: QCefView
 * Created: 13th July 2022
 * Author: Sheen
 * Source: https://github.com/cefview/qcefview
 * Docs: https://cefview.github.io/QCefView/
 */
#ifndef QCEFDOWNLOADITEM_H
#define QCEFDOWNLOADITEM_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QDateTime>
#include <QScopedPointer>
#include <QString>
#pragma endregion 

class QCefDownloadItemPrivate;

/// <summary>
/// Represents the download item
/// </summary>
class QCEFVIEW_EXPORT QCefDownloadItem
{
  Q_DECLARE_PRIVATE(QCefDownloadItem)
  QScopedPointer<QCefDownloadItemPrivate> d_ptr;

  Q_DISABLE_COPY(QCefDownloadItem)
  QCefDownloadItem(QCefDownloadItem&&) = delete;
  QCefDownloadItem& operator=(QCefDownloadItem&&) = delete;

  friend class QCefView;

  /// <summary>
  /// Constructs a download item instance
  /// </summary>
  QCefDownloadItem();

public:
  /// <summary>
  /// Destructs the download item instance
  /// </summary>
  ~QCefDownloadItem();

  /// <summary>
  /// Starts to download the item.
  /// </summary>
  /// <param name="path">The full path name (must include file name) to save the downloaded item</param>
  /// <param name="useDefaultDialog">Whether to use the default 'Save As...' dialog or not</param>
  /// <remarks>
  /// The 'path' parameter only works when 'useDefaultDialog' is set to false.
  /// If you set 'useDefaultDialog' to true then you cannot control the initial
  /// location of the opened 'Save As...' dialog, it is determined by CEF internal implementation.
  /// </remarks>
  void start(const QString& path, bool useDefaultDialog = true) const;

  /// <summary>
  /// Pauses the download
  /// </summary>
  void pause() const;

  /// <summary>
  /// Resume the download
  /// </summary>
  void resume() const;

  /// <summary>
  /// Cancels the download
  /// </summary>
  void cancel() const;

  /// <summary>
  /// Gets whether the download is started
  /// </summary>
  /// <returns></returns>
  bool isStarted() const;

  /// <summary>
  /// Gets whether the download is in progress
  /// </summary>
  /// <returns>True if the download is in progress; otherwise false</returns>
  bool isInProgress() const;

  /// <summary>
  /// Gets whether the download is complete
  /// </summary>
  /// <returns>True if the download is complete; otherwise false</returns>
  bool isComplete() const;

  /// <summary>
  /// Gets whether the download has been canceled or interrupted
  /// </summary>
  /// <returns>True if the download has been canceled or interrupted; otherwise false</returns>
  bool isCanceled() const;

  /// <summary>
  /// Gets current download speed
  /// </summary>
  /// <returns>A simple speed estimate in bytes/s</returns>
  qint64 currentSpeed() const;

  /// <summary>
  /// Gets the completion percentage
  /// </summary>
  /// <returns>The rough percent complete or -1 if the receive total size is unknown</returns>
  int percentComplete() const;

  /// <summary>
  /// Gets total number of bytes
  /// </summary>
  /// <returns>The total number of bytes</returns>
  qint64 totalBytes() const;

  /// <summary>
  /// Gets number of received bytes
  /// </summary>
  /// <returns>The number of received bytes</returns>
  qint64 receivedBytes() const;

  /// <summary>
  /// Gets the time that the download started
  /// </summary>
  /// <returns>The time that the download started</returns>
  QDateTime startTime() const;

  /// <summary>
  /// Gets the time that the download ended
  /// </summary>
  /// <returns>The time that the download ended</returns>
  QDateTime endTime() const;

  /// <summary>
  /// Gets the full path to the downloaded or downloading file
  /// </summary>
  /// <returns>The full path to the downloaded or downloading file</returns>
  QString fullPath() const;

  /// <summary>
  /// Gets the unique identifier for this download
  /// </summary>
  /// <returns>The unique identifier for this download</returns>
  quint32 id() const;

  /// <summary>
  /// Gets the URL
  /// </summary>
  /// <returns>The the URL</returns>
  QString url() const;

  /// <summary>
  /// Gets the original URL before any redirections
  /// </summary>
  /// <returns>The original URL before any redirections</returns>
  QString originalUrl() const;

  /// <summary>
  /// Gets the suggested file name
  /// </summary>
  /// <returns>The suggested file name</returns>
  QString suggestedFileName() const;

  /// <summary>
  /// Gets the content disposition
  /// </summary>
  /// <returns>The the content disposition</returns>
  QString contentDisposition() const;

  /// <summary>
  /// Gets the mime type
  /// </summary>
  /// <returns>The mime type</returns>
  QString mimeType() const;
};

#endif /// QCEFDOWNLOADITEM_H
