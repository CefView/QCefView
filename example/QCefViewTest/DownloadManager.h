#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#pragma once

#include <QMap>
#include <QSharedPointer>

#include <QCefDownloadItem.h>

class DownloadManager
{
public:
  static DownloadManager& getInstance();

  void AddNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item);

  void UpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item);

private:
  DownloadManager();
  ~DownloadManager();

  QMap<qint32, QSharedPointer<QCefDownloadItem>> m_mapDownloadingItem;
};

#endif
