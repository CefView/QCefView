#include "DownloadManager.h"

#include <QDebug>

DownloadManager&
DownloadManager::getInstance()
{
  static DownloadManager s_instance;
  return s_instance;
}

void
DownloadManager::AddNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item)
{
  qDebug() << "DownloadManager::AddNewDownloadItem:"
           << "  id: " << item->id() << "\n"
           << "  name: " << item->suggestedFileName() << "\n"
           << "  path: " << item->fullPath() << "\n"
           << "  percent: " << item->percentComplete() << "%, " << item->totalBytes() << "/" << item->receivedBytes()
           << "\n"
           << "  canceled: " << item->isCanceled() << "\n"
           << "  complete: " << item->isComplete();

  m_mapDownloadingItem[item->id()] = item;
  item->start("", true);
}

void
DownloadManager::UpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item)
{
  qDebug() << "DownloadManager::UpdateDownloadItem:"
           << "  id: " << item->id() << "\n"
           << "  name: " << item->suggestedFileName() << "\n"
           << "  path: " << item->fullPath() << "\n"
           << "  percent: " << item->percentComplete() << "%, " << item->totalBytes() << "/" << item->receivedBytes()
           << "\n"
           << "  canceled: " << item->isCanceled() << "\n"
           << "  complete: " << item->isComplete();

  if (item->isCanceled() || item->isComplete())
    m_mapDownloadingItem.remove(item->id());
}

DownloadManager::DownloadManager() {}

DownloadManager::~DownloadManager()
{
  for (auto& item : m_mapDownloadingItem) {
    item->cancel();
  }
  m_mapDownloadingItem.clear();
}
