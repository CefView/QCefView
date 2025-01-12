#include "CefViewWidget.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#elif defined(Q_OS_MAC)
#elif defined(Q_OS_LINUX)
#else
#endif

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
  // setStyleSheet("background: blue;");

  connect(this, &CefViewWidget::draggableRegionChanged, this, &CefViewWidget::onDraggableRegionChanged);
  connect(this, &CefViewWidget::nativeBrowserCreated, this, &CefViewWidget::onNativeBrowserWindowCreated);
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
CefViewWidget::onNativeBrowserWindowCreated(QWindow* window)
{
  m_pCefWindow = window;
  if (!m_pCefWindow)
    return;

  connect(this->window()->windowHandle(), SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));

  updateMask();
}

void
CefViewWidget::onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion)
{
  m_draggableRegion = draggableRegion;
  m_nonDraggableRegion = nonDraggableRegion;
}

bool
CefViewWidget::onNewPopup(const QCefFrameId& sourceFrameId,
                          const QString& targetUrl,
                          QString& targetFrameName,
                          QCefView::CefWindowOpenDisposition targetDisposition,
                          QRect& rect,
                          QCefSetting& settings,
                          bool& disableJavascriptAccess)
{
  // create new QCefView as popup browser
  // settings.setBackgroundColor(Qt::red);
  return false;
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

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  // update mask first, because the new mask will be
  // used in the QCefView::resizeEvent
  updateMask();

  QCefView::resizeEvent(event);
}

void
CefViewWidget::mousePressEvent(QMouseEvent* event)
{
  QCefView::mousePressEvent(event);

#if defined(Q_OS_WIN)
  if (event->buttons().testFlag(Qt::LeftButton) && m_draggableRegion.contains(event->pos())) {
    HWND hWnd = ::GetAncestor((HWND)(window()->windowHandle()->winId()), GA_ROOT);
    POINT pt;
    ::GetCursorPos(&pt);
    ::ReleaseCapture();
    ::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, POINTTOPOINTS(pt));
  }
#elif defined(Q_OS_MAC)
#elif defined(Q_OS_LINUX)
#else
#endif
}

void
CefViewWidget::updateMask()
{
  // create a rect with rounded corner (50px radius) as mask
  QPainterPath path;
  path.addRoundedRect(rect(), 50, 50);
  QRegion mask = QRegion(path.toFillPolygon().toPolygon());

  // apply the mask
  setMask(mask);
}
