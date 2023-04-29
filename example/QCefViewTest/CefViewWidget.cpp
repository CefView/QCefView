#include "CefViewWidget.h"

#include <QColor>
#include <QDebug>
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


bool
CefViewWidget::onBeforePopup(qint64 frameId,
                             const QString& targetUrl,
                             const QString& targetFrameName,
                             QCefView::CefWindowOpenDisposition targetDisposition,
                             QCefSetting& settings,
                             bool& DisableJavascriptAccess)
{
  qDebug() << "onBeforePopup\n"
           << "frameId:" << frameId << "\n "
           << "targetUrl:" << targetUrl << "\n"
           << "targetFrameName:" << targetFrameName;

  // return true to disallow the popup browser
  // return true;

  // here we can modify the settings for the popup browsers
  settings.setBackgroundColor(Qt::red);

  // return false to allow the popup browser
  return false;
}

void
CefViewWidget::onPopupCreated(QWindow* wnd)
{
  // customize the window style of the created popup browser
  // This not recommended, because when this method gets called
  // the popup browser window has been created and displayed already
  // if you modify the window style and re-parent it, the window will
  // get flicker quickly

  // QWidget* popupWindow = QWidget::createWindowContainer(wnd, nullptr, Qt::Window);
  // popupWindow->setAttribute(Qt::WA_DeleteOnClose, true);
  // popupWindow->show();
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
