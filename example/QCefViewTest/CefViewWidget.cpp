#include "CefViewWidget.h"

#include <QColor>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

#include "DownloadManager.h"

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{
}

CefViewWidget::~CefViewWidget() {}

void
CefViewWidget::onScreenChanged(QScreen* screen)
{
  if (!m_pCefWindow)
    return;

  updateMask();
}

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  m_pCefWindow = win;
  if (!m_pCefWindow)
    return;

  connect(window()->windowHandle(), SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));

  updateMask();
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);
  updateMask();
}

void
CefViewWidget::updateMask()
{
  QPainterPath path;
  path.addRoundedRect(rect(), 50, 50);
  QRegion mask = QRegion(path.toFillPolygon().toPolygon());
  setMask(mask);
}

void
CefViewWidget::onNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item, const QString& suggestedName)
{
  // keep the item into list or map, and call item->start() to allow the download

  DownloadManager::getInstance().AddNewDownloadItem(item);
}

void
CefViewWidget::onUpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item)
{

  // control the download by invoking item->pause(), item->resume(), item->cancel()

  DownloadManager::getInstance().UpdateDownloadItem(item);
}
