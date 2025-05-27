#include "details/CCefClientDelegate.h"

#include <QBitmap>
#include <QByteArray>
#include <QCursor>
#include <QIcon>

#include "details/utils/CursorUtils.h"

#include "details/QCefViewPrivate.h"

void
CCefClientDelegate::addressChanged(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, const CefString& url)
{
  if (!IsValidBrowser(browser))
    return;

  auto u = QString::fromStdString(url);
  emit pCefViewPrivate_->q_ptr->addressChanged(ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), u);
}

void
CCefClientDelegate::titleChanged(CefRefPtr<CefBrowser>& browser, const CefString& title)
{
  if (!IsValidBrowser(browser))
    return;

  auto t = QString::fromStdString(title);
  emit pCefViewPrivate_->q_ptr->titleChanged(t);
}

void
CCefClientDelegate::faviconURLChanged(CefRefPtr<CefBrowser>& browser, const std::vector<CefString>& icon_urls)
{
  if (!IsValidBrowser(browser))
    return;

  QStringList urls;
  for (auto& iconUrl : icon_urls) {
    urls.append(QString::fromStdString(iconUrl.ToString()));
  }

  emit pCefViewPrivate_->q_ptr->faviconURLChanged(urls);
}

void
CCefClientDelegate::fullscreenModeChanged(CefRefPtr<CefBrowser>& browser, bool fullscreen)
{
  if (!IsValidBrowser(browser))
    return;

  emit pCefViewPrivate_->q_ptr->fullscreenModeChanged(fullscreen);
}

bool
CCefClientDelegate::tooltipMessage(CefRefPtr<CefBrowser>& browser, const CefString& text)
{
  // allow the tooltip action
  return false;
}

void
CCefClientDelegate::statusMessage(CefRefPtr<CefBrowser>& browser, const CefString& value)
{
  if (!IsValidBrowser(browser))
    return;

  auto msg = QString::fromStdString(value);
  emit pCefViewPrivate_->q_ptr->statusMessage(msg);
}

void
CCefClientDelegate::consoleMessage(CefRefPtr<CefBrowser>& browser, const CefString& message, int level)
{
  if (!IsValidBrowser(browser))
    return;

  auto msg = QString::fromStdString(message);
  emit pCefViewPrivate_->q_ptr->consoleMessage(msg, level);
}

void
CCefClientDelegate::loadingProgressChanged(CefRefPtr<CefBrowser>& browser, double progress)
{
  if (!IsValidBrowser(browser))
    return;

  emit pCefViewPrivate_->q_ptr->loadingProgressChanged(progress);
}

bool
CCefClientDelegate::cursorChanged(CefRefPtr<CefBrowser>& browser,
                                  CefCursorHandle cursor,
                                  cef_cursor_type_t type,
                                  const CefCursorInfo& custom_cursor_info)
{
  if (!IsValidBrowser(browser))
    return false;

  if (pCefViewPrivate_->isOSRModeEnabled_) {
    // OSR mode
    QCursor cur;
    if (type != CT_CUSTOM) {
      // create cursor from shape
      cur = QCursor(MapCursorShape(type));
    } else {
      // create cursor from image data
      cur = QCursor(QPixmap::fromImage(QImage(static_cast<const uchar*>(custom_cursor_info.buffer),
                                              custom_cursor_info.size.width,
                                              custom_cursor_info.size.height,
                                              QImage::Format_ARGB32_Premultiplied)));
    }

    QMetaObject::invokeMethod(pCefViewPrivate_, "onCefUpdateCursor", Q_ARG(const QCursor&, cur));

    return true;
  } else {
    return false;
  }
}
