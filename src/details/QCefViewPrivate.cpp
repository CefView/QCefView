#include "QCefViewPrivate.h"

#pragma region std_headers
#include <stdexcept>
#pragma endregion std_headers

#pragma region qt_headers
#include <QApplication>
#include <QDebug>
#include <QInputMethodQueryEvent>
#include <QPainter>
#include <QPlatformSurfaceEvent>
#include <QVBoxLayout>
#include <QWindow>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_parser.h>
#pragma endregion cef_headers

#include <CefViewCoreProtocol.h>

#include "CCefClientDelegate.h"
#include "QCefContext.h"
#include "QCefSettingPrivate.h"
#include "ValueConvertor.h"

QSet<QCefViewPrivate*> QCefViewPrivate::sLiveInstances;

void
QCefViewPrivate::destroyAllInstance()
{
  auto s = sLiveInstances;
  for (auto& i : s) {
    i->destroyCefBrowser();
  }
}

QCefViewPrivate::QCefViewPrivate(QCefView* view, const QString& url, const QCefSetting* setting)
  : q_ptr(view)
  , pContextPrivate_(QCefContext::instance()->d_func())
  , pClient_(nullptr)
  , pClientDelegate_(nullptr)
  , pCefBrowser_(nullptr)
{
  sLiveInstances.insert(this);

  createCefBrowser(view, url, setting);
}

QCefViewPrivate::~QCefViewPrivate()
{
  destroyCefBrowser();

  sLiveInstances.remove(this);
}

void
QCefViewPrivate::createCefBrowser(QCefView* view, const QString url, const QCefSetting* setting)
{
  // create browser client handler delegate
  auto pClientDelegate = std::make_shared<CCefClientDelegate>(this);

  // create browser client handler
  auto pClient = new CefViewBrowserClient(pContextPrivate_->getCefApp(), pClientDelegate);

  for (auto& folderMapping : pContextPrivate_->folderResourceMappingList()) {
    pClient->AddLocalDirectoryResourceProvider(
      folderMapping.path.toStdString(), folderMapping.url.toStdString(), folderMapping.priority);
  }

  for (auto& archiveMapping : pContextPrivate_->archiveResourceMappingList()) {
    pClient->AddArchiveResourceProvider(archiveMapping.path.toStdString(),
                                        archiveMapping.url.toStdString(),
                                        archiveMapping.password.toStdString(),
                                        archiveMapping.priority);
  }

  // Set window info
  CefWindowInfo window_info;
  window_info.SetAsWindowless((CefWindowHandle)view->winId());

  // create the browser settings
  CefBrowserSettings browserSettings;
  browserSettings.windowless_frame_rate = 60;
  if (setting) {
    QCefSettingPrivate::CopyToCefBrowserSettings(setting, &browserSettings);
  }

  // create browser object
  bool success = CefBrowserHost::CreateBrowser(window_info,       // window info
                                               pClient,           // handler
                                               url.toStdString(), // url
                                               browserSettings,   // settings
                                               nullptr,
                                               CefRequestContext::GetGlobalContext());
  Q_ASSERT_X(success, "QCefViewPrivate::createBrowser", "Failed to create cef browser");
  if (!success) {
    qWarning("Failed to create cef browser");
    return;
  }

  QApplication::instance()->installEventFilter(this);

  pClient_ = pClient;
  pClientDelegate_ = pClientDelegate;
  return;
}

void
QCefViewPrivate::destroyCefBrowser()
{
  if (!pClient_)
    return;

  // clean all browsers
  pClient_->CloseAllBrowsers();

  pClient_ = nullptr;
  pCefBrowser_ = nullptr;
}

void
QCefViewPrivate::invalidateRender()
{
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->Invalidate(PET_VIEW);
}

void
QCefViewPrivate::onCefBrowserCreated(CefRefPtr<CefBrowser>& browser)
{
  pCefBrowser_ = browser;

  QMetaObject::invokeMethod(
    this,
    [=]() {
      q_ptr->installEventFilter(this);
      qApp->installNativeEventFilter(this);
    },
    Qt::BlockingQueuedConnection);
}

void
QCefViewPrivate::onInputStateChanged(bool editable)
{
  Q_Q(QCefView);

  q->setAttribute(Qt::WA_InputMethodEnabled, editable);
}

void
QCefViewPrivate::onImeCursorRectChanged(const QRect& rc)
{
  qImeCursorRect_ = rc;
  auto inputMethod = QGuiApplication::inputMethod();
  if (inputMethod) {
    inputMethod->update(Qt::ImCursorRectangle);
  }
}

void
QCefViewPrivate::onUpdateCursor(const QCursor& cursor)
{
  Q_Q(QCefView);

  q->setCursor(cursor);
}

void
QCefViewPrivate::onCefWindowLostTabFocus(bool next)
{
  // The focus was returned from CEF window, QCefView needs to handle
  // this event and give the focus to the correct next or previous widget
  Q_Q(QCefView);

  auto reason = next ? Qt::TabFocusReason : Qt::BacktabFocusReason;
  auto widget = next ? q->nextInFocusChain() : q->previousInFocusChain();

  // find correct widget
  while (widget && 0 == (widget->focusPolicy() & Qt::TabFocus)) {
    widget = next ? widget->nextInFocusChain() : widget->previousInFocusChain();
  }

  if (widget) {
    // TO-DO bug: this does not work on Linux(X11), need to find a workaround
    widget->window()->raise();
    widget->setFocus(reason);
  }
}

void
QCefViewPrivate::onCefWindowGotFocus()
{}

void
QCefViewPrivate::onShowPopup(bool show)
{
  showPopup_ = show;
}

void
QCefViewPrivate::onResizePopup(const QRect& rc)
{
  qPopupRect_ = rc;
}

void
QCefViewPrivate::onCefViewFrame(const QImage& frame, const QRegion& region)
{
  Q_Q(QCefView);

  // copy to hold the frame buffer data
  auto framePixmap = QPixmap::fromImage(frame.copy());
  QMetaObject::invokeMethod(q, [=]() {
    qCefViewFrame_ = framePixmap;
    q->update();
  });
}

void
QCefViewPrivate::onCefPopupFrame(const QImage& frame, const QRegion& region)
{
  Q_Q(QCefView);

  // copy to hold the frame buffer data
  auto framePixmap = QPixmap::fromImage(frame.copy());
  QMetaObject::invokeMethod(q, [=]() {
    qCefPopupFrame_ = framePixmap;
    q->update();
  });
}

bool
QCefViewPrivate::eventFilter(QObject* watched, QEvent* event)
{
  Q_Q(QCefView);

  if (watched != q)
    return QObject::eventFilter(watched, event);

  switch (event->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
      onMouseEvent((QMouseEvent*)event);
      break;
    case QEvent::Wheel:
      onWheelEvent((QWheelEvent*)event);
      break;
    case QEvent::InputMethod:
      onInputMethodEvent((QInputMethodEvent*)event);
      return true;
    default:
      break;
  }

  return QObject::eventFilter(watched, event);
}

void
QCefViewPrivate::onVisibilityChanged(bool visible)
{
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->WasHidden(!visible);
}

void
QCefViewPrivate::onFocusChanged(bool focused)
{
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->SetFocus(focused);
}

void
QCefViewPrivate::onSizeChanged(const QSize& size, const QSize& oldSize)
{
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->WasResized();
}

void
QCefViewPrivate::onMouseEvent(QMouseEvent* event)
{
  if (!pCefBrowser_)
    return;

  auto m = event->modifiers();
  auto b = event->buttons();

  CefMouseEvent e;
  e.modifiers |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;
  e.modifiers |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
  e.modifiers |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
  e.modifiers |= b & Qt::LeftButton ? EVENTFLAG_LEFT_MOUSE_BUTTON : 0;
  e.modifiers |= b & Qt::RightButton ? EVENTFLAG_RIGHT_MOUSE_BUTTON : 0;
  e.modifiers |= b & Qt::MiddleButton ? EVENTFLAG_MIDDLE_MOUSE_BUTTON : 0;
  e.x = ((QMouseEvent*)event)->pos().x();
  e.y = ((QMouseEvent*)event)->pos().y();

  if (QEvent::MouseMove == event->type()) {
    pCefBrowser_->GetHost()->SendMouseMoveEvent(e, false);
    return;
  }

  CefBrowserHost::MouseButtonType mbt = MBT_LEFT;
  switch (event->button()) {
    case Qt::LeftButton: {
      mbt = MBT_LEFT;
    } break;
    case Qt::RightButton: {
      mbt = MBT_RIGHT;
    } break;
    case Qt::MiddleButton: {
      mbt = MBT_MIDDLE;
    } break;
    default:
      break;
  }

  if (QEvent::MouseButtonPress == event->type()) {
    pCefBrowser_->GetHost()->SendMouseClickEvent(e, mbt, false, event->pointCount());
  } else if (QEvent::MouseButtonRelease == event->type()) {
    // if the release was generated right after a popup, we must discard it
    pCefBrowser_->GetHost()->SendMouseClickEvent(e, mbt, true, event->pointCount());
  }
}

void
QCefViewPrivate::onWheelEvent(QWheelEvent* event)
{
  auto p = event->position();
  auto d = event->angleDelta();
  auto m = event->modifiers();
  auto b = event->buttons();

  CefMouseEvent e;
  e.modifiers |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;
  e.modifiers |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
  e.modifiers |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
  e.modifiers |= b & Qt::LeftButton ? EVENTFLAG_LEFT_MOUSE_BUTTON : 0;
  e.modifiers |= b & Qt::RightButton ? EVENTFLAG_RIGHT_MOUSE_BUTTON : 0;
  e.modifiers |= b & Qt::MiddleButton ? EVENTFLAG_MIDDLE_MOUSE_BUTTON : 0;

  e.x = p.x();
  e.y = p.y();
  pCefBrowser_->GetHost()->SendMouseWheelEvent(e, m & Qt::ShiftModifier ? 0 : d.x(), m & Qt::ShiftModifier ? 0 : d.y());
}

void
QCefViewPrivate::onInputMethodEvent(QInputMethodEvent* event)
{
  if (!pCefBrowser_)
    return;

  auto composingText = event->preeditString();
  auto composedText = event->commitString();

  if (!composedText.isEmpty()) {
    pCefBrowser_->GetHost()->ImeCommitText(composedText.toStdString(), CefRange(UINT32_MAX, UINT32_MAX), 0);
  } else if (!composingText.isEmpty()) {
    CefCompositionUnderline underline;
    underline.background_color = 0;
    underline.range = { 0, (int)composingText.length() };

    CefRange selectionRange;
    for (auto& attr : event->attributes()) {
      switch (attr.type) {
        case QInputMethodEvent::TextFormat:
          break;
        case QInputMethodEvent::Cursor:
          selectionRange.Set(attr.start, attr.start);
          break;
        case QInputMethodEvent::Language:
        case QInputMethodEvent::Ruby:
        case QInputMethodEvent::Selection:
          break;
        default:
          break;
      }
    }
    pCefBrowser_->GetHost()->ImeSetComposition(
      composingText.toStdString(), { underline }, CefRange(UINT32_MAX, UINT32_MAX), selectionRange);
  } else {
    pCefBrowser_->GetHost()->ImeCancelComposition();
  }
}

int
QCefViewPrivate::browserId()
{
  if (pCefBrowser_)
    return pCefBrowser_->GetIdentifier();

  return -1;
}

void
QCefViewPrivate::navigateToString(const QString& content)
{
  if (pCefBrowser_) {
    std::string data = content.toStdString();
    data = CefURIEncode(CefBase64Encode(data.c_str(), data.size()), false).ToString();
    data = "data:text/html;base64," + data;
    pCefBrowser_->GetMainFrame()->LoadURL(data);
  }
}

void
QCefViewPrivate::navigateToUrl(const QString& url)
{
  if (pCefBrowser_) {
    CefString strUrl;
    strUrl.FromString(url.toStdString());
    pCefBrowser_->GetMainFrame()->LoadURL(strUrl);
  }
}

bool
QCefViewPrivate::browserCanGoBack()
{
  if (pCefBrowser_)
    return pCefBrowser_->CanGoBack();

  return false;
}

bool
QCefViewPrivate::browserCanGoForward()
{
  if (pCefBrowser_)
    return pCefBrowser_->CanGoForward();

  return false;
}

void
QCefViewPrivate::browserGoBack()
{
  if (pCefBrowser_)
    pCefBrowser_->GoBack();
}

void
QCefViewPrivate::browserGoForward()
{
  if (pCefBrowser_)
    pCefBrowser_->GoForward();
}

bool
QCefViewPrivate::browserIsLoading()
{
  if (pCefBrowser_)
    return pCefBrowser_->IsLoading();

  return false;
}

void
QCefViewPrivate::browserReload()
{
  if (pCefBrowser_)
    pCefBrowser_->Reload();
}

void
QCefViewPrivate::browserStopLoad()
{
  if (pCefBrowser_)
    pCefBrowser_->StopLoad();
}

bool
QCefViewPrivate::triggerEvent(const QString& name,
                              const QVariantList& args,
                              int64_t frameId /*= CefViewBrowserHandler::MAIN_FRAME*/)
{
  if (!name.isEmpty()) {
    return sendEventNotifyMessage(frameId, name, args);
  }

  return false;
}

bool
QCefViewPrivate::responseQCefQuery(const QCefQuery& query)
{
  if (pClient_) {
    CefString res;
    res.FromString(query.response().toStdString());
    return pClient_->ResponseQuery(query.id(), query.result(), res, query.error());
  }
  return false;
}

bool
QCefViewPrivate::executeJavascript(int64_t frameId, const QString& code, const QString& url)
{
  if (code.isEmpty())
    return false;

  if (pCefBrowser_) {
    CefRefPtr<CefFrame> frame = pCefBrowser_->GetFrame(frameId);
    if (frame) {
      CefString c;
      c.FromString(code.toStdString());

      CefString u;
      if (url.isEmpty()) {
        u = frame->GetURL();
      } else {
        u.FromString(url.toStdString());
      }

      frame->ExecuteJavaScript(c, u, 0);

      return true;
    }
  }

  return false;
}

bool
QCefViewPrivate::executeJavascriptWithResult(int64_t frameId, const QString& code, const QString& url, int64_t context)
{
  if (code.isEmpty())
    return false;

  if (pClient_) {
    auto frame = frameId == 0 ? pCefBrowser_->GetMainFrame() : pCefBrowser_->GetFrame(frameId);
    if (!frame)
      return false;

    CefString c;
    c.FromString(code.toStdString());

    CefString u;
    if (url.isEmpty()) {
      u = frame->GetURL();
    } else {
      u.FromString(url.toStdString());
    }

    return pClient_->AsyncExecuteJSCode(pCefBrowser_, frame, c, u, context);
  }

  return false;
}

void
QCefViewPrivate::notifyMoveOrResizeStarted()
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host)
      host->NotifyMoveOrResizeStarted();
  }
}

bool
QCefViewPrivate::sendEventNotifyMessage(int64_t frameId, const QString& name, const QVariantList& args)
{
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(kCefViewClientBrowserTriggerEventMessage);
  CefRefPtr<CefListValue> arguments = msg->GetArgumentList();

  //** arguments(CefValueList)
  //** +------------+
  //** | event name |
  //** | event arg1 |
  //** | event arg2 |
  //** | event arg3 |
  //** | event arg4 |
  //** |    ...     |
  //** |    ...     |
  //** |    ...     |
  //** |    ...     |
  //** +------------+
  int idx = 0;
  CefString eventName = name.toStdString();
  arguments->SetString(idx++, eventName);
  for (auto& qV : args) {
    auto cVal = CefValue::Create();
    ValueConvertor::QVariantToCefValue(cVal.get(), &qV);
    arguments->SetValue(idx++, cVal);
  }

  return pClient_->TriggerEvent(pCefBrowser_, frameId, msg);
}

bool
QCefViewPrivate::setPreference(const QString& name, const QVariant& value, const QString& error)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      CefString n;
      n.FromString(name.toStdString());

      auto v = CefValue::Create();
      ValueConvertor::QVariantToCefValue(v.get(), &value);

      CefString e;
      auto r = host->GetRequestContext()->SetPreference(n, v, e);
      error.fromStdString(e.ToString());
      return r;
    }
  }

  return false;
}
