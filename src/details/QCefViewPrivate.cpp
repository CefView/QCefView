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
#if defined(Q_OS_LINUX) && !defined(CEF_USE_OSR)
#include <X11/Xlib.h>
#include <qpa/qplatformnativeinterface.h>
#endif
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
#include "utils/KeyboardUtils.h"
#include "utils/ValueConvertor.h"

#if defined(Q_OS_LINUX) && !defined(CEF_USE_OSR)
Display*
X11GetDisplay(QWidget* widget)
{
  Q_ASSERT_X(widget, "X11GetDisplay", "Invalid parameter widget");
  if (!widget) {
    qWarning("Invalid parameter widget");
    return nullptr;
  }

  auto platformInterface = QApplication::platformNativeInterface();
  Q_ASSERT_X(platformInterface, "X11GetDisplay", "Failed to get platform native interface");
  if (!platformInterface) {
    qWarning("Failed to get platform native interface");
    return nullptr;
  }

  auto screen = widget->window()->windowHandle()->screen();
  Q_ASSERT_X(screen, "X11GetDisplay", "Failed to get screen");
  if (!screen) {
    qWarning("Failed to get screen");
    return nullptr;
  }

  return (Display*)platformInterface->nativeResourceForScreen("display", screen);
}
#endif

QSet<QCefViewPrivate*> QCefViewPrivate::sLiveInstances;

void
QCefViewPrivate::destroyAllInstance()
{
  auto s = sLiveInstances;
  for (auto& i : s) {
    i->destroyCefBrowser();
  }
}

QCefViewPrivate::QCefViewPrivate(QCefContextPrivate* ctx,
                                 QCefView* view,
                                 const QString& url,
                                 const QCefSetting* setting)
  : q_ptr(view)
  , pContextPrivate_(ctx)
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
#if defined(CEF_USE_OSR)
  window_info.SetAsWindowless(nullptr);
#else
  window_info.SetAsChild((CefWindowHandle)view->winId(), { 0, 0, view->maximumWidth(), view->maximumHeight() });
#endif

  // create the browser settings
  CefBrowserSettings browserSettings;
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

  // install global event filter
  qApp->installEventFilter(this);

  pClient_ = pClient;
  pClientDelegate_ = pClientDelegate;
  return;
}

void
QCefViewPrivate::destroyCefBrowser()
{
  if (!pClient_)
    return;

#if !defined(CEF_USE_OSR)
  // remove parent, or CEF will send close to the parent
  // this will lead the top level window to be closed
  ncw.qBrowserWindow_->setParent(nullptr);
#endif

  // clean all browsers
  pClient_->CloseAllBrowsers();
  pClient_ = nullptr;
  pCefBrowser_ = nullptr;
}

void
QCefViewPrivate::onCefMainBrowserCreated(CefRefPtr<CefBrowser>& browser, QWindow* window)
{
  // capture the browser
  pCefBrowser_ = browser;

#if defined(CEF_USE_OSR)
  // notify the visibility and size
  pCefBrowser_->GetHost()->WasHidden(!q_ptr->isVisible());
  pCefBrowser_->GetHost()->WasResized();
  connect(this, SIGNAL(updateOsrFrame()), q_ptr, SLOT(update()));
  connect(q_ptr->windowHandle(), SIGNAL(screenChanged(QScreen*)), this, SLOT(onViewScreenChanged(QScreen*)));
#else
  // create QWidget from cef browser widow, this will re-parent the CEF browser window
  QWidget* browserWidget = QWidget::createWindowContainer(
    window,
    q_ptr,
    Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
  Q_ASSERT_X(browserWidget, "QCefViewPrivateNCW::createBrowser", "Failed to create QWidget from cef browser window");
  if (!browserWidget) {
    qWarning("Failed to create QWidget from cef browser window");
    browser->GetHost()->CloseBrowser(true);
    return;
  }

  // capture the resource
  ncw.qBrowserWindow_ = window;
  ncw.qBrowserWidget_ = browserWidget;

  // monitor the focus changed event globally
  connect(qApp, &QApplication::focusChanged, this, &QCefViewPrivate::onAppFocusChanged);

  // initialize the layout and add browser widget to the layout
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(ncw.qBrowserWidget_);
  q_ptr->setLayout(layout);
#endif
}

void
QCefViewPrivate::onCefPopupBrowserCreated(CefRefPtr<CefBrowser>& browser, QWindow* window)
{
  Q_Q(QCefView);

  q->onPopupCreated(window);
}

bool
QCefViewPrivate::onCefDoCloseBrowser(CefRefPtr<CefBrowser>& browser)
{
  return false;
}

void
QCefViewPrivate::onCefBeforeCloseBrowser(CefRefPtr<CefBrowser>& browser)
{
  return;
}

void
QCefViewPrivate::setCefWindowFocus(bool focus)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->SetFocus(focus);
    }
  }
}

void
QCefViewPrivate::onAppFocusChanged(QWidget* old, QWidget* now)
{
  Q_Q(QCefView);

  // qDebug() << q << q->window()->isActiveWindow() << ":focus changed from:" << old << " -> " << now;
  // qDebug() << q->windowHandle() << "focusWindow:" << QGuiApplication::focusWindow();

  if (!now || now->window() != q->window())
    return;

  if (now == q) {
    // QCefView got focus, need to move the focus to the CEF browser window
    // This only works when changing focus by TAB key
    if (old && old->window() == q->window())
      setCefWindowFocus(true);
  } else {
    // Because setCefWindowFocus will not release CEF browser window focus,
    // here we need to activate the new focused widget forcefully.
    // This code should be executed only when click any focus except
    // the QCefView while QCefView is holding the focus
    if (!old && !QGuiApplication::focusWindow())
      now->activateWindow();
  }
}

void
QCefViewPrivate::onViewScreenChanged(QScreen* screen)
{
#if defined(CEF_USE_OSR)
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->NotifyScreenInfoChanged();
#endif
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
    widget->activateWindow();
  }
}

void
QCefViewPrivate::onCefWindowGotFocus()
{
  // qDebug() << "cefwindow got focus";
  Q_Q(QCefView);
  QWidget* focusedWidget = qApp->focusWidget();
  if (focusedWidget && focusedWidget != q) {
    focusedWidget->clearFocus();
  }
  return;
}

void
QCefViewPrivate::onCefUpdateCursor(const QCursor& cursor)
{
#if defined(CEF_USE_OSR)
  Q_Q(QCefView);
  q->setCursor(cursor);
#endif
}

void
QCefViewPrivate::onCefInputStateChanged(bool editable)
{
  Q_Q(QCefView);
  q->setAttribute(Qt::WA_InputMethodEnabled, editable);
}

#if defined(CEF_USE_OSR)
void
QCefViewPrivate::onOsrImeCursorRectChanged(const QRect& rc)
{
  osr.qImeCursorRect_ = rc;
  auto inputMethod = QGuiApplication::inputMethod();
  if (inputMethod) {
    inputMethod->update(Qt::ImCursorRectangle);
  }
}

void
QCefViewPrivate::onOsrShowPopup(bool show)
{
  osr.showPopup_ = show;
}

void
QCefViewPrivate::onOsrResizePopup(const QRect& rc)
{
  osr.qPopupRect_ = rc;
}

void
QCefViewPrivate::onOsrUpdateViewFrame(const QImage& frame, const QRegion& region)
{
  if (osr.qCefViewFrame_.size() == frame.size()) {
    QMutexLocker lock(&(osr.qViewPaintLock_));
    // update region
    QPainter painter(&osr.qCefViewFrame_);
    for (auto& rc : region) {
      painter.drawImage(rc, frame, rc);
    }
  } else {
    QMutexLocker lock(&(osr.qViewPaintLock_));
    osr.qCefViewFrame_ = frame.copy();
  }
  emit updateOsrFrame();
}

void
QCefViewPrivate::onOsrUpdatePopupFrame(const QImage& frame, const QRegion& region)
{
  if (osr.qCefPopupFrame_.size() == frame.size()) {
    QMutexLocker lock(&(osr.qPopupPaintLock_));
    // update region
    QPainter painter(&osr.qCefPopupFrame_);
    for (auto& rc : region) {
      painter.drawImage(rc, frame, rc);
    }
  } else {
    QMutexLocker lock(&(osr.qPopupPaintLock_));
    osr.qCefPopupFrame_ = frame.copy();
  }
  emit updateOsrFrame();
}
#endif

bool
QCefViewPrivate::eventFilter(QObject* watched, QEvent* event)
{
  Q_Q(QCefView);

  auto et = event->type();

#if !defined(Q_OS_MACOS)
  // monitor the move event of the top-level window and the widget
  if ((watched == q || watched == q->window()) && (et == QEvent::Move || et == QEvent::Resize)) {
    notifyMoveOrResizeStarted();
  }
#endif

#if defined(CEF_USE_OSR)
  if (watched == q && (et == QEvent::KeyPress || et == QEvent::KeyRelease)) {
    QKeyEvent* ke = (QKeyEvent*)event;
    if (ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Backtab) {
      onViewKeyEvent(ke);
      return true;
    }
  }
#else
  // filter event to the browser window
  if (watched == ncw.qBrowserWindow_ && et == QEvent::PlatformSurface) {
    auto t = ((QPlatformSurfaceEvent*)event)->surfaceEventType();
    if (QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed == t) {
      // browser window is being destroyed, need to close the browser window in advance
      destroyCefBrowser();
    }
  }
#endif

  return QObject::eventFilter(watched, event);
}

QVariant
QCefViewPrivate::onViewInputMethodQuery(Qt::InputMethodQuery query) const
{
#if defined(CEF_USE_OSR)
  switch (query) {
    case Qt::ImCursorRectangle:
      return QVariant(osr.qImeCursorRect_);
    case Qt::ImAnchorRectangle:
      break;
    case Qt::ImFont:
      break;
    case Qt::ImCursorPosition:
      break;
    case Qt::ImSurroundingText:
      break;
    case Qt::ImCurrentSelection:
      break;
    case Qt::ImMaximumTextLength:
      break;
    case Qt::ImAnchorPosition:
      break;
    default:
      break;
  }
#endif
  return QVariant();
}

void
QCefViewPrivate::onViewInputMethodEvent(QInputMethodEvent* event)
{
#if defined(CEF_USE_OSR)
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
#endif
}

void
QCefViewPrivate::onViewVisibilityChanged(bool visible)
{
#if defined(CEF_USE_OSR)
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->WasHidden(!visible);
#else
  Q_Q(QCefView);
  if (ncw.qBrowserWidget_) {
    if (visible) {
#if defined(Q_OS_LINUX) && !defined(CEF_USE_OSR)
      if (::XMapWindow(X11GetDisplay(ncw.qBrowserWidget_), ncw.qBrowserWindow_->winId()) <= 0)
        qWarning() << "Failed to move input focus";
        // BUG-TO-BE-FIXED after remap, the browser window will not resize automatically
        // with the QCefView widget
#endif
      ncw.qBrowserWidget_->resize(q->frameSize());
    } else {
      ncw.qBrowserWidget_->resize(0, 0);
    }
  }
#endif
}

void
QCefViewPrivate::onViewFocusChanged(bool focused)
{
#if defined(CEF_USE_OSR)
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->SetFocus(focused);
#endif
}

void
QCefViewPrivate::onViewSizeChanged(const QSize& size, const QSize& oldSize)
{
#if defined(CEF_USE_OSR)
  if (pCefBrowser_)
    pCefBrowser_->GetHost()->WasResized();
#endif
}

void
QCefViewPrivate::onViewKeyEvent(QKeyEvent* event)
{
#if defined(CEF_USE_OSR)
  if (!pCefBrowser_)
    return;

  // qDebug("==== onViewKeyEvent:key=%d, nativeVirtualKey=0x%02x, nativeScanCode=0x%02x, modifiers=0x%08x, "
  //       "nativeModifiers=0x%08x, text=%s",
  //       (Qt::Key)(event->key()),
  //       event->nativeVirtualKey(),
  //       event->nativeScanCode(),
  //       (quint32)(event->modifiers()),
  //       event->nativeModifiers(),
  //       event->text().toStdString().c_str());

  CefKeyEvent e;
  MapQKeyEventToCefKeyEvent(event, e);

  // QEvent::KeyRelease - send key release event
  if (event->type() == QEvent::KeyRelease) {
    e.type = KEYEVENT_KEYUP;
    pCefBrowser_->GetHost()->SendKeyEvent(e);
    return;
  }

  // QEvent::KeyPress - send key down event
  e.type = KEYEVENT_RAWKEYDOWN;
  pCefBrowser_->GetHost()->SendKeyEvent(e);

  // send key char event
  if (ShouldSendKeyCharEvent(event)) {
    AdjustCefKeyCharEvent(event, e);
    e.type = KEYEVENT_CHAR;
    pCefBrowser_->GetHost()->SendKeyEvent(e);
  }
#endif
}

void
QCefViewPrivate::onViewMouseEvent(QMouseEvent* event)
{
#if defined(CEF_USE_OSR)
  if (!pCefBrowser_)
    return;

  auto b = event->buttons();
  auto m = event->modifiers();

  CefMouseEvent e;
  e.x = ((QMouseEvent*)event)->pos().x();
  e.y = ((QMouseEvent*)event)->pos().y();
  e.modifiers |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;
  e.modifiers |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
  e.modifiers |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
  e.modifiers |= b & Qt::LeftButton ? EVENTFLAG_LEFT_MOUSE_BUTTON : 0;
  e.modifiers |= b & Qt::RightButton ? EVENTFLAG_RIGHT_MOUSE_BUTTON : 0;
  e.modifiers |= b & Qt::MiddleButton ? EVENTFLAG_MIDDLE_MOUSE_BUTTON : 0;

  if (QEvent::MouseMove == event->type()) {
    pCefBrowser_->GetHost()->SendMouseMoveEvent(e, false);
    return;
  }

  // qDebug() << "====== onViewMouseEvent:" << event;

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
    pCefBrowser_->GetHost()->SendMouseClickEvent(e, mbt, false, 1);
  } else if (QEvent::MouseButtonDblClick == event->type()) {
    pCefBrowser_->GetHost()->SendMouseClickEvent(e, mbt, false, 2);
  } else if (QEvent::MouseButtonRelease == event->type()) {
    // if the release was generated right after a popup, we must discard it
    pCefBrowser_->GetHost()->SendMouseClickEvent(e, mbt, true, 1);
  }
#endif
}

void
QCefViewPrivate::onViewWheelEvent(QWheelEvent* event)
{
#if defined(CEF_USE_OSR)
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
#endif
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
    if (host) {
      host->NotifyMoveOrResizeStarted();
    }
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
