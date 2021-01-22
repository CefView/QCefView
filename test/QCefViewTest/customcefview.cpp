#if defined(OS_WINDOWS)
#include <windows.h>
#else
#endif

#include <QMessageBox>
#include <QColor>
#include <QRandomGenerator>

#include "customcefview.h"

CustomCefView::~CustomCefView() {}

void
CustomCefView::changeColor()
{
  QColor color(QRandomGenerator::global()->generate());

  QCefEvent event("colorChange");
  event.setStringProperty("color", color.name());
  broadcastEvent(event);
}

void
CustomCefView::onDraggableRegionChanged(const QRegion& region)
{}

void
CustomCefView::onQCefUrlRequest(const QString& url)
{
  QMetaObject::invokeMethod(
    this,
    [=]() {
      QString title("QCef URL Request");
      QString text = QString("Current Thread: QT_UI\r\n"
                             "URL: %1")
                       .arg(url);

      QMessageBox::information(this->window(), title, text);
    },
    Qt::QueuedConnection);
}

void
CustomCefView::onQCefQueryRequest(const QCefQuery& query)
{
  QMetaObject::invokeMethod(
    this,
    [=]() {
      QString title("QCef Query Request");
      QString text = QString("Current Thread: QT_UI\r\n"
                             "Query: %1")
                       .arg(query.reqeust());

      QMessageBox::information(this->window(), title, text);

      QString response = query.reqeust().toUpper();
      query.setResponseResult(true, response);
      responseQCefQuery(query);
    },
    Qt::QueuedConnection);
}

void
CustomCefView::onInvokeMethodNotify(int browserId, int frameId, const QString& method, const QVariantList& arguments)
{
  if (0 == method.compare("onDragAreaMouseDown")) {
#if defined(OS_WINDOWS)
    HWND hWnd = ::GetAncestor((HWND)(this->window()->winId()), GA_ROOT);
    // get current mouse cursor position
    POINT pt;
    ::GetCursorPos(&pt);
    // in case the mouse is being captured, try to release it
    ::ReleaseCapture();
    // simulate that the mouse left button is down on the title area
    ::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, POINTTOPOINTS(pt));
#endif
    return;
  } else if (0 == method.compare("TestMethod")) {
    QMetaObject::invokeMethod(
      this,
      [=]() {
        QString title("QCef InvokeMethod Notify");
        QString text = QString("Current Thread: QT_UI\r\n"
                               "Method: %1\r\n"
                               "Arguments: ...")
                         .arg(method);

        QMessageBox::information(this->window(), title, text);
      },
      Qt::QueuedConnection);
  } else {
  }
}
