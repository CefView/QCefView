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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // workaround for:
  // https://github.com/chromiumembedded/cef/issues/3870
  // after navigation CEF resets the browser focus status
  // without any callback notification (AKA, released the
  // focus silently), so we need to update the CEF browser
  // focus status according to the one we have kept
  if (true                                    //
      && pCefViewPrivate->isOSRModeEnabled_   //
      && pCefViewPrivate->osr.hasCefGotFocus_ //
      && browser->GetHost()                   //
  ) {
    browser->GetHost()->SetFocus(true);
  }

  auto f = ValueConvertor::FrameIdC2Q(frame->GetIdentifier());
  auto u = QString::fromStdString(url);

  runInMainThread([pCefViewPrivate, f, u]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->addressChanged(f, u);
    }
  });
}

void
CCefClientDelegate::titleChanged(CefRefPtr<CefBrowser>& browser, const CefString& title)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  auto t = QString::fromStdString(title);

  runInMainThread([pCefViewPrivate, t]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->titleChanged(t);
    }
  });
}

void
CCefClientDelegate::faviconURLChanged(CefRefPtr<CefBrowser>& browser, const std::vector<CefString>& icon_urls)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  QStringList urls;
  for (auto& iconUrl : icon_urls) {
    urls.append(QString::fromStdString(iconUrl.ToString()));
  }

  runInMainThread([pCefViewPrivate, urls]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->faviconURLChanged(urls);
    }
  });
}

void
CCefClientDelegate::fullscreenModeChanged(CefRefPtr<CefBrowser>& browser, bool fullscreen)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread([pCefViewPrivate, fullscreen]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->fullscreenModeChanged(fullscreen);
    }
  });
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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  auto msg = QString::fromStdString(value);

  runInMainThread([pCefViewPrivate, msg]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->statusMessage(msg);
    }
  });
}

void
CCefClientDelegate::consoleMessage(CefRefPtr<CefBrowser>& browser, const CefString& message, int level)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  auto msg = QString::fromStdString(message);

  runInMainThread([pCefViewPrivate, msg, level]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->consoleMessage(msg, level);
    }
  });
}

void
CCefClientDelegate::loadingProgressChanged(CefRefPtr<CefBrowser>& browser, double progress)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread([pCefViewPrivate, progress]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->loadingProgressChanged(progress);
    }
  });
}

bool
CCefClientDelegate::cursorChanged(CefRefPtr<CefBrowser>& browser,
                                  CefCursorHandle cursor,
                                  cef_cursor_type_t type,
                                  const CefCursorInfo& custom_cursor_info)
{
  AcquireAndValidateCefViewPrivateWithReturn(pCefViewPrivate, false);

  if (pCefViewPrivate->isOSRModeEnabled_) {
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

    runInMainThread(             //
      [pCefViewPrivate, cur]() { //
        pCefViewPrivate->onCefUpdateCursor(cur);
      });

    return true;
  }

  return false;
}
