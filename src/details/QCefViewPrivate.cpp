﻿#include "QCefViewPrivate.h"

#pragma region stl_headers
#include <stdexcept>
#pragma endregion

#pragma region qt_headers
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDrag>
#include <QFileDialog>
#include <QGridLayout>
#include <QInputMethodQueryEvent>
#include <QPainter>
#include <QStyleOption>
#include <QWindow>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_parser.h>
#pragma endregion

#include <CefViewCoreProtocol.h>

#include "CCefClientDelegate.h"
#include "QCefContext.h"
#include "QCefSettingPrivate.h"
#include "utils/CommonUtils.h"
#include "utils/DragAndDropUtils.h"
#include "utils/KeyboardUtils.h"
#include "utils/MenuBuilder.h"
#include "utils/ValueConvertor.h"

QSet<QCefViewPrivate*> QCefViewPrivate::sLiveInstances;

void
QCefViewPrivate::destroyAllInstance()
{
  auto s = sLiveInstances;
  for (auto& i : s) {
    i->destroyCefBrowser();
  }
}

QCefViewPrivate::QCefViewPrivate(QCefContextPrivate* ctx, QCefView* view)
  : q_ptr(view)
  , pContextPrivate_(ctx)
{
  sLiveInstances.insert(this);

  isOSRModeEnabled_ = pContextPrivate_->cefConfig()->windowlessRenderingEnabled().toBool();
}

QCefViewPrivate::~QCefViewPrivate()
{
  sLiveInstances.remove(this);
}

void
QCefViewPrivate::createCefBrowser(const QString& url, const QCefSettingPrivate* setting)
{
  // 1. create browser client handler delegate
  auto pClientDelegate = std::make_shared<CCefClientDelegate>(this);

  // 2. create browser client handler
  auto pClient = new CefViewBrowserClient(pContextPrivate_->getCefApp(), pClientDelegate);

  // 3. create the browser settings
  CefBrowserSettings browserSettings;
  QCefSettingPrivate::CopyToCefBrowserSettings(setting, &browserSettings);

  // 4. set window info
  CefWindowInfo windowInfo;
  if (isOSRModeEnabled_) {
    auto winSize = q_ptr->size();
    // set QWidget background
    QColor background = q_ptr->palette().color(q_ptr->backgroundRole());
    CefViewRendererPtr renderer;
    // if hardware is enabled
    if (setting && setting->hardwareAcceleration_) {
#if CEF_VERSION_MAJOR >= 125
      // create hardware renderer if enabled
      if ((renderer = CefViewRendererFactory::createRenderer(true))) {
        // initialize renderer
        if (renderer->initialize(q_ptr,            //
                                 winSize.width(),  //
                                 winSize.height(), //
                                 scaleFactor(),    //
                                 background.rgba() //
                                 )) {
          // OSR with hardware renderer
          osr.pRenderer_ = renderer;
          windowInfo.SetAsWindowless(0);
          windowInfo.shared_texture_enabled = true;
        } else {
          qWarning() << "Failed to initialize hardware renderer, will fall back to software renderer";
        }
      } else {
        qWarning() << "Failed to create hardware renderer, will fall back to software renderer";
      }
    } else {
      qInfo() << "Hardware Acceleration is disabled";
#else
      qWarning() << "Hardware Acceleration is supported only on CEF version 125+";
#endif
    }

    // if hardware renderer is not enabled or failed to create it
    // fall back to software renderer
    if (!osr.pRenderer_) {
      // create software renderer
      if ((renderer = CefViewRendererFactory::createRenderer(false))) {
        // initialize renderer
        if (renderer->initialize(q_ptr,            //
                                 winSize.width(),  //
                                 winSize.height(), //
                                 scaleFactor(),    //
                                 background.rgba() //
                                 )) {
          // OSR with software renderer
          osr.pRenderer_ = renderer;
          windowInfo.SetAsWindowless(0);
          windowInfo.shared_texture_enabled = false;
        } else {
          qWarning() << "Failed to initialize software renderer";
        }
      } else {
        qWarning() << "Failed to create software renderer";
      }
    }

    Q_ASSERT_X(                            //
      osr.pRenderer_,                      //
      "QCefViewPrivate::createCefBrowser", //
      "Failed to create osr renderer"      //
    );
    if (!osr.pRenderer_) {
      // all renderer were failed to create or initialize
      qCritical() << "QCefViewPrivate::createCefBrowser: Failed to create osr renderer.";
    }
  } else {
#if CEF_VERSION_MAJOR >= 125
    // https://github.com/chromiumembedded/cef/issues/3685
    windowInfo.runtime_style = CEF_RUNTIME_STYLE_ALLOY;
#endif
    // create CEF browser parent window
    auto windowInitialSize = q_ptr->size();
    if (setting) {
      windowInitialSize = setting->windowInitialSize_;
    }
    qDebug() << "Browser init size:" << windowInitialSize;

    ncw.qBrowserWindow_ = new QCefWindow();
    ncw.qBrowserWindow_->resize(windowInitialSize);
    ncw.qBrowserWindow_->setFlags(Qt::Window | Qt::FramelessWindowHint);

    auto width = windowInitialSize.width() * scaleFactor();
    auto height = windowInitialSize.height() * scaleFactor();
#if CEF_VERSION_MAJOR > 85
    windowInfo.SetAsChild((CefWindowHandle)ncw.qBrowserWindow_->winId(), { 0, 0, (int)width, (int)height });
#else
    windowInfo.SetAsChild((CefWindowHandle)ncw.qBrowserWindow_->winId(), 0, 0, (int)width, (int)height);
#endif
  }

  // 5. create browser object
  bool success = CefBrowserHost::CreateBrowser(windowInfo,        // window info
                                               pClient,           // handler
                                               url.toStdString(), // url
                                               browserSettings,   // settings
                                               nullptr,
                                               CefRequestContext::GetGlobalContext());
  Q_ASSERT_X(                         //
    success,                          //
    "QCefViewPrivate::createBrowser", //
    "Failed to create cef browser"    //
  );
  if (!success) {
    qCritical() << "QCefViewPrivate::createBrowser: Failed to create cef browser";
    return;
  }

  // install global event filter
  qApp->installEventFilter(this);

  pClient_ = pClient;
  pClientDelegate_ = pClientDelegate;
}

void
QCefViewPrivate::destroyCefBrowser()
{
  qDebug() << "destroy browser from native";

  if (!pClient_)
    return;

  if (!isOSRModeEnabled_) {
    // remove from parent, prevent from being destroyed
    ncw.qBrowserWidget_->setParent(nullptr);
    ncw.qBrowserWidget_->deleteLater();
    ncw.qBrowserWindow_->detachCefWindow();
  }

  // clean all browsers
  pClient_->CloseAllBrowsers();
  pClient_ = nullptr;
  pCefBrowser_ = nullptr;
}

void
QCefViewPrivate::addLocalFolderResource(const QString& path, const QString& url, int priority /*= 0*/)
{
  if (pClient_) {
    pClient_->AddLocalDirectoryResourceProvider(path.toStdString(), url.toStdString(), priority);
  }
}

void
QCefViewPrivate::addArchiveResource(const QString& path,
                                    const QString& url,
                                    const QString& password /*= ""*/,
                                    int priority /*= 0*/)
{
  if (pClient_) {
    pClient_->AddArchiveResourceProvider(path.toStdString(), url.toStdString(), password.toStdString(), priority);
  }
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

QCefQuery
QCefViewPrivate::createQuery(const QString& req, const int64_t id)
{
  return QCefQuery(this, req, id);
}

void
QCefViewPrivate::onCefBrowserCreated(CefRefPtr<CefBrowser> browser, QWindow* window)
{
  // capture the browser
  pCefBrowser_ = browser;

  // if last url is not empty, need to navigate to it
  if (!lastUrl_.empty()) {
    browser->GetMainFrame()->LoadURL(lastUrl_);
  }

  // monitor the screenChanged signal from the top-level window
  if (q_ptr->window()->windowHandle()) {
    connect(q_ptr->window()->windowHandle(),     //
            SIGNAL(screenChanged(QScreen*)),     //
            this,                                //
            SLOT(onViewScreenChanged(QScreen*)), //
            Qt::UniqueConnection                 //
    );
  }

  if (isOSRModeEnabled_) {
    // OSR mode
    // notify the visibility and size
    pCefBrowser_->GetHost()->WasHidden(!q_ptr->isVisible());
    pCefBrowser_->GetHost()->WasResized();
  } else {
    // emit signal
    emit q_ptr->nativeBrowserCreated(window);

    qDebug() << "CEF Window Native ID:" << window->winId();

    // create widget for cef window
    ncw.qBrowserWidget_ = ncw.qBrowserWindow_->attachCefWindow(window, q_ptr);
    Q_ASSERT_X(                                                //
      ncw.qBrowserWidget_,                                     //
      "QCefViewPrivate::onCefBrowserCreated",                  //
      "Failed to create alien QWidget from cef browser window" //
    );
    if (!ncw.qBrowserWidget_) {
      qCritical() << "QCefViewPrivate::onCefBrowserCreated: Failed to create QWidget from cef browser window";
      browser->GetHost()->CloseBrowser(true);
      return;
    }

    // adjust size/mask and attach to cef window
    ncw.qBrowserWindow_->applyMask(q_ptr->mask());

    // resize to eliminate flicker
    qDebug() << "Host QCefView size:" << q_ptr->size();
    ncw.qBrowserWidget_->resize(q_ptr->size());

    // initialize the layout and add browser widget to the layout
    QGridLayout* layout = new QGridLayout(q_ptr);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(ncw.qBrowserWidget_);
  }

#if defined(QT_DEBUG)
  //  monitor the focus changed event globally
  connect(qApp,                        //
          &QApplication::focusChanged, //
          [](QWidget* old, QWidget* now) {
            qDebug() << "Focus changed from:"                                            //
                     << old << "[" << (old ? old->window()->windowHandle() : 0x0) << "]" //
                     << "->"                                                             //
                     << now << "[" << (now ? now->window()->windowHandle() : 0x00) << "]";
          });
#endif
}

bool
QCefViewPrivate::onBeforeNewBrowserCreate(const QCefFrameId& sourceFrameId,
                                          const QString& targetUrl,
                                          const QString& targetFrameName,
                                          QCefView::CefWindowOpenDisposition targetDisposition,
                                          QRect rect,
                                          QCefSetting settings)
{
  Q_Q(QCefView);

  // this is a fake pop-up browser, we just cancel it and then
  // we create a new QCefView instance to replace the fake pop-up browser
  q->onNewBrowser(sourceFrameId,     //
                  targetUrl,         //
                  targetFrameName,   //
                  targetDisposition, //
                  rect,              //
                  settings);
  return true;
}

bool
QCefViewPrivate::onBeforeNewPopupCreate(const QCefFrameId& sourceFrameId,
                                        const QString& targetUrl,
                                        QString& targetFrameName,
                                        QCefView::CefWindowOpenDisposition targetDisposition,
                                        QRect& rect,
                                        QCefSetting& settings,
                                        bool& disableJavascriptAccess)
{
  Q_Q(QCefView);

  return q->onNewPopup(sourceFrameId,     //
                       targetUrl,         //
                       targetFrameName,   //
                       targetDisposition, //
                       rect,              //
                       settings,          //
                       disableJavascriptAccess);
}

void
QCefViewPrivate::onAfterCefPopupCreated(CefRefPtr<CefBrowser> browser)
{
}

void
QCefViewPrivate::onNewDownloadItem(QSharedPointer<QCefDownloadItem> item, const QString& suggestedName)
{
  Q_Q(QCefView);

  q->onNewDownloadItem(item, suggestedName);
}

void
QCefViewPrivate::onUpdateDownloadItem(QSharedPointer<QCefDownloadItem> item)
{
  Q_Q(QCefView);

  q->onUpdateDownloadItem(item);
}

bool
QCefViewPrivate::handleLoadError(CefRefPtr<CefBrowser>& browser,
                                 CefRefPtr<CefFrame>& frame,
                                 int errorCode,
                                 const std::string& errorMsg,
                                 const std::string& failedUrl)
{
  Q_Q(QCefView);

  // If the signal was connected then emit the signal and set handled with true to skip the default handler
  if (q->receivers(SIGNAL(loadError(int, qint64, bool, int, const QString&, const QString&))) > 0) {
    auto msg = QString::fromStdString(errorMsg);
    auto url = QString::fromStdString(failedUrl);
    emit q->loadError(browser->GetIdentifier(),
                      ValueConvertor::FrameIdC2Q(frame->GetIdentifier()),
                      frame->IsMain(),
                      errorCode,
                      msg,
                      url);
    return true;
  }

  return false;
}

bool
QCefViewPrivate::requestCloseFromWeb(CefRefPtr<CefBrowser>& browser)
{
  Q_Q(QCefView);

  return q->onRequestCloseFromWeb();
}

qreal
QCefViewPrivate::scaleFactor()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
  qreal scaleFactor = q_ptr->devicePixelRatioF();
#else
  qreal scaleFactor = q_ptr->devicePixelRatio();
#endif
  return scaleFactor;
}

void
QCefViewPrivate::onViewScreenChanged(QScreen* screen)
{
  Q_Q(QCefView);

  // no matter what reason, we need to update the geometry
  // because some system/Qt version will not notify the new
  // size information correctly when screen change
  q->updateGeometry();

  if (isOSRModeEnabled_) {
    // OSR mode
    if (pCefBrowser_) {
      pCefBrowser_->GetHost()->WasResized();
      pCefBrowser_->GetHost()->NotifyScreenInfoChanged();
    }
  } else {
    if (ncw.qBrowserWindow_)
      ncw.qBrowserWindow_->applyMask(q->mask());
  }
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
  qDebug() << "----- QCefViewPrivate::onCefWindowGotFocus()";
  Q_Q(QCefView);

  if (isOSRModeEnabled_) {
    // OSR mode, sync focus status
    if (!q->hasFocus()) {
      q->setFocus();
    }
  } else {
    // NCW mode, clear previous focus
    QWidget* focusWidget = qApp->focusWidget();
    if (focusWidget && focusWidget != q) {
      focusWidget->clearFocus();
    }
  }
}

void
QCefViewPrivate::onCefUpdateCursor(const QCursor& cursor)
{
  if (isOSRModeEnabled_) {
    // OSR mode
    Q_Q(QCefView);
    q->setCursor(cursor);
  }
}

void
QCefViewPrivate::onCefInputStateChanged(bool editable)
{
  Q_Q(QCefView);
  q->setAttribute(Qt::WA_InputMethodEnabled, editable);
}

void
QCefViewPrivate::onOsrImeCursorRectChanged(const QRect& rc)
{
  osr.imeCursorRect_ = rc;
  auto inputMethod = QGuiApplication::inputMethod();
  if (inputMethod) {
    inputMethod->update(Qt::ImCursorRectangle);
  }
}

void
QCefViewPrivate::onContextMenuTriggered(QAction* action)
{
  FLog();

  if (action && osr.contextMenuCallback_) {
    // TO-DO (sheen) retrieve correct event flags
    auto eventFlags = EVENTFLAG_NONE;
    auto commandId = action->data().toInt();
    osr.contextMenuCallback_->Continue(commandId, eventFlags);
    osr.contextMenuCallback_ = nullptr;
  }
}

void
QCefViewPrivate::onContextMenuDestroyed(QObject* obj)
{
  FLog();

  if (osr.contextMenu_) {
    osr.contextMenu_ = nullptr;
  }

  if (osr.contextMenuCallback_) {
    osr.contextMenuCallback_->Cancel();
    osr.contextMenuCallback_ = nullptr;
  }

  osr.isShowingContextMenu_ = false;
}

void
QCefViewPrivate::onStartDragging(CefRefPtr<CefDragData>& dragData, CefRenderHandler::DragOperationsMask allowedOps)
{
  Q_Q(QCefView);

  Qt::DropAction dropAction = Qt::IgnoreAction;

  // create drag
  if (QMimeData* mimeData = CreateQMimeDataFromCefDragData(*dragData)) {
    QDrag drag(q);
    drag.setMimeData(mimeData);

    CefRefPtr<CefImage> image = dragData->GetImage();
    if (image) {
      int w = 0;
      int h = 0;
      if (auto pngData = image->GetAsPNG(1.0, true, w, h)) {
        QPixmap pixmap;
        pixmap.loadFromData((const uchar*)(pngData->GetRawData()), (int)(pngData->GetSize()));
        pixmap.setDevicePixelRatio(scaleFactor());
        drag.setPixmap(pixmap);
      }

      CefPoint hotspot = dragData->GetImageHotspot();
      drag.setHotSpot(QPoint(hotspot.x, hotspot.y));
    }

    // execute dragging
    osr.allowedDragOperations_ = MapCefDragOperationToQDropAction(allowedOps);
    dropAction = drag.exec(static_cast<Qt::DropActions>(osr.allowedDragOperations_), Qt::CopyAction);
  }

  // notify cef that the drag ends
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      auto result = MapQDropActionToCefDragOperation(dropAction);
      auto cursor = q->mapFromGlobal(QCursor::pos());
      host->DragSourceEndedAt(cursor.x(), cursor.y(), result);
      host->DragSourceSystemDragEnded();
    }
  }
}

void
QCefViewPrivate::onBeforeCefContextMenu(const MenuBuilder::MenuData& data)
{
  Q_Q(QCefView);

  // clear previous context menu
  if (osr.contextMenu_) {
    osr.contextMenu_->close();
    osr.contextMenu_->deleteLater();
    osr.contextMenu_ = nullptr;
  }

  // create context menu
  osr.contextMenu_ = new QMenu(q);
  osr.contextMenu_->setAttribute(Qt::WA_DeleteOnClose);

  // connect context menu signals
  connect(osr.contextMenu_, SIGNAL(triggered(QAction*)), this, SLOT(onContextMenuTriggered(QAction*)));
  connect(osr.contextMenu_, SIGNAL(destroyed(QObject*)), this, SLOT(onContextMenuDestroyed(QObject*)));

  // build menu from data
  MenuBuilder::BuildQtMenuFromMenuData(osr.contextMenu_, data);
}

void
QCefViewPrivate::onRunCefContextMenu(QPoint pos, CefRefPtr<CefRunContextMenuCallback> callback)
{
  Q_Q(QCefView);

  // keep the context menu callback
  osr.contextMenuCallback_ = callback;

  // show context menu
  osr.isShowingContextMenu_ = true;
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
  QContextMenuEvent* e = new QContextMenuEvent(QContextMenuEvent::Other, pos);
#else
  QPoint globalPos = q->mapToGlobal(pos);
  QContextMenuEvent* e =
    new QContextMenuEvent(QContextMenuEvent::Other, pos, globalPos, QGuiApplication::keyboardModifiers());
#endif
  qApp->sendEvent(q, e);
}

void
QCefViewPrivate::onCefContextMenuDismissed()
{
  osr.contextMenuCallback_ = nullptr;
}

void
QCefViewPrivate::onFileDialog(CefBrowserHost::FileDialogMode mode,
                              const QString& title,
                              const QString& default_file_path,
                              const QStringList& accept_filters,
#if CEF_VERSION_MAJOR < 102
                              int selected_accept_filter,
#endif
                              CefRefPtr<CefFileDialogCallback> callback)
{
  Q_Q(QCefView);

  // create dialog and set mode
  QFileDialog dialog(q_ptr);
  if (mode == FILE_DIALOG_OPEN) {
    dialog.setFileMode(QFileDialog::ExistingFile);
  } else if (mode == FILE_DIALOG_OPEN_MULTIPLE) {
    dialog.setFileMode(QFileDialog::ExistingFiles);
  } else if (mode == FILE_DIALOG_OPEN_FOLDER) {
    dialog.setFileMode(QFileDialog::Directory);
  } else if (mode == FILE_DIALOG_SAVE) {
    dialog.setAcceptMode(QFileDialog::AcceptSave);
  } else {
    callback->Cancel();
    return;
  }

  // set title
  if (!title.isEmpty()) {
    dialog.setWindowTitle(title);
  }

  // set initial folder
  if (!default_file_path.isEmpty() && mode == FILE_DIALOG_SAVE) {
    QFileInfo fileInfo(default_file_path);
    dialog.setDirectory(fileInfo.dir());
    dialog.selectFile(fileInfo.fileName());
  }

  // set accepted file types
  dialog.setNameFilters(accept_filters);

  // execute the dialog
  if (dialog.exec()) {
    std::vector<CefString> file_paths;
    auto selected_files = dialog.selectedFiles();
    for (const auto& file : selected_files) {
      file_paths.push_back(QDir::toNativeSeparators(file).toStdString());
    }

#if CEF_VERSION_MAJOR < 102
    int index = accept_filters.indexOf(dialog.selectedNameFilter());
    callback->Continue(index, file_paths);
#else
    callback->Continue(file_paths);
#endif
  } else {
    callback->Cancel();
  }
}

bool
QCefViewPrivate::hasDevTools()
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      return host->HasDevTools();
    }
  }

  return false;
}

void
QCefViewPrivate::showDevTools()
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      CefWindowInfo info;
      CefBrowserSettings settings;
      CefPoint point;
      host->ShowDevTools(info, nullptr, settings, point);
    }
  }
}

void
QCefViewPrivate::closeDevTools()
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->CloseDevTools();
    }
  }
}

bool
QCefViewPrivate::shouldAllowDrop(CefRefPtr<CefDragData>& dragData, CefDragHandler::DragOperationsMask mask)
{
  Q_Q(QCefView);

  return q->acceptDrops();
}

bool
QCefViewPrivate::shouldAllowDrag(CefRefPtr<CefDragData>& dragData,
                                 CefRenderHandler::DragOperationsMask allowedOps,
                                 int x,
                                 int y)
{
  // This method only works for OSR mode
  if (!allowDrag_) {
    // return false to cancel the drag operation
    return false;
  }

  // start dragging
  QMetaObject::invokeMethod(
    this, //
    [this, dragData, allowedOps]() mutable {
      //
      this->onStartDragging(dragData, allowedOps);
    },                   //
    Qt::QueuedConnection //
  );
  return true;
}

void
QCefViewPrivate::updateDragOperation(CefRenderHandler::DragOperationsMask allowedOps)
{
  if (isOSRModeEnabled_) {
    osr.allowedDragOperations_ = MapCefDragOperationToQDropAction(allowedOps);
  }
}

bool
QCefViewPrivate::eventFilter(QObject* watched, QEvent* event)
{
  Q_Q(QCefView);

  switch (event->type()) {
    case QEvent::Move: {
      // if any ancestor was moved we need to notify CEF
      QWidget* w = qobject_cast<QWidget*>(watched);
      if (w && w->isAncestorOf(q)) {
        notifyMoveOrResizeStarted();
      }
    } break;
    case QEvent::ParentChange: {
      // if any ancestor's parent was changed,
      // we need to reconnect the screenChanged signal/slots
      QWidget* w = qobject_cast<QWidget*>(watched);
      if (w && w->isAncestorOf(q)) {
        // disconnect old
        disconnect(SIGNAL(screenChanged(QScreen*)),    //
                   this,                               //
                   SLOT(onViewScreenChanged(QScreen*)) //
        );

        // connect new
        if (q->window()->windowHandle()) {
          connect(q->window()->windowHandle(),        //
                  SIGNAL(screenChanged(QScreen*)),    //
                  this,                               //
                  SLOT(onViewScreenChanged(QScreen*)) //
          );
        }
      }
    } break;
    case QEvent::PlatformSurface: {
      if (!isOSRModeEnabled_) {
        // the surface is about to be destroyed (top-level window is being closed)
        auto t = static_cast<QPlatformSurfaceEvent*>(event)->surfaceEventType();
        if (QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed == t) {
          if (watched == ncw.qBrowserWindow_->cefWindow()) {
            // detach the cef window
            ncw.qBrowserWindow_->detachCefWindow();
          }
        }
      }
    } break;
    case QEvent::FocusIn: {
      if (!isOSRModeEnabled_ && watched == ncw.qBrowserWindow_) {
        qDebug() << "----- event to ncw.qBrowserWindow_:" << event;
        q->focusInEvent(static_cast<QFocusEvent*>(event));
      }
    } break;
    case QEvent::FocusOut: {
      if (!isOSRModeEnabled_ && watched == ncw.qBrowserWindow_) {
        qDebug() << "----- event to ncw.qBrowserWindow_:" << event;
        q->focusOutEvent(static_cast<QFocusEvent*>(event));
      }
    } break;
    default:
      break;
  }

  return QObject::eventFilter(watched, event);
}

QVariant
QCefViewPrivate::onViewInputMethodQuery(Qt::InputMethodQuery query) const
{
  if (isOSRModeEnabled_) {
    // OSR mode
    switch (query) {
      case Qt::ImCursorRectangle:
        return QVariant(osr.imeCursorRect_);
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
  }
  return QVariant();
}

void
QCefViewPrivate::onViewInputMethodEvent(QInputMethodEvent* event)
{
  if (isOSRModeEnabled_) {
    // OSR mode
    if (!pCefBrowser_)
      return;

    auto composingText = event->preeditString();
    auto composedText = event->commitString();

    if (!composedText.isEmpty()) {
      pCefBrowser_->GetHost()->ImeCommitText(composedText.toStdString(), CefRange(UINT32_MAX, UINT32_MAX), 0);
    } else if (!composingText.isEmpty()) {
      CefCompositionUnderline underline;
      underline.background_color = 0;
      underline.range = { 0, static_cast<decltype(CefRange::to)>(composingText.length()) };

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
}

void
QCefViewPrivate::onViewVisibilityChanged(bool visible)
{
  if (q_ptr->window()->windowHandle()) {
    connect(q_ptr->window()->windowHandle(),     //
            SIGNAL(screenChanged(QScreen*)),     //
            this,                                //
            SLOT(onViewScreenChanged(QScreen*)), //
            Qt::UniqueConnection);
  }

  // tell cef to start/stop rendering
  if (isOSRModeEnabled_) {
    // OSR mode
    if (pCefBrowser_)
      pCefBrowser_->GetHost()->WasHidden(!visible);
  } else {
    // if (!ncw.qBrowserWidget_)
    //   return;

    //// for better user experience (eliminate flicker), we don't change the size here
    //// if the consumer of QCefView need to constraint the resource
    //// then they need to handle the size change themselves
    // if (visible) {
    //   // restore cef window size
    //   ncw.qBrowserWidget_->resize(q->frameSize());
    // } else {
    //   // set cef window size to 0x0 to stop rendering and reduce resource usage
    //   // please refer to:
    //   // https://bitbucket.org/chromiumembedded/cef/issues/2310/allow-cefbrowser-washidden-to-work-for
    //   ncw.qBrowserWidget_->resize(0, 0);
    // }
  }
}

void
QCefViewPrivate::onViewFocusChanged(bool focused)
{
  if (!pCefBrowser_)
    return;

  if (isOSRModeEnabled_) {
    // if context menu is showing we need to skip the focus out event
    if (osr.isShowingContextMenu_ && !focused)
      return;

    // sync focus status to CEF
    pCefBrowser_->GetHost()->SetFocus(focused);
  } else {
    // sync focus status to CEF
    pCefBrowser_->GetHost()->SetFocus(focused);
  }
}

void
QCefViewPrivate::onPaintEvent(QPaintEvent* event)
{
  Q_Q(QCefView);

  if (osr.pRenderer_ && pCefBrowser_) {
    // invalidate the CEF OSR buffer
    pCefBrowser_->GetHost()->Invalidate(CefBrowserHost::PaintElementType::PET_VIEW);
  }
}

void
QCefViewPrivate::onViewMoved()
{
  notifyMoveOrResizeStarted();
}

void
QCefViewPrivate::onViewSizeChanged(const QSize& size, const QSize& oldSize)
{
  Q_Q(QCefView);

  notifyMoveOrResizeStarted();

  if (isOSRModeEnabled_) {
    // OSR mode
    if (pCefBrowser_) {
      // invalidate the CEF OSR buffer
      pCefBrowser_->GetHost()->Invalidate(CefBrowserHost::PaintElementType::PET_VIEW);
    }

    if (osr.pRenderer_) {
      // resize render
      osr.pRenderer_->resize(size.width(), size.height(), scaleFactor());
    }

    // OSR mode
    if (pCefBrowser_) {
      // notify CEF of the size changing
      pCefBrowser_->GetHost()->WasResized();
    }
  } else {
    if (ncw.qBrowserWindow_) {
      ncw.qBrowserWindow_->applyMask(q->mask());
    }
  }
}

void
QCefViewPrivate::onViewKeyEvent(QKeyEvent* event)
{
  if (isOSRModeEnabled_) {
    // OSR mode
    if (!pCefBrowser_)
      return;

    // #if defined(QT_DEBUG)
    //     qDebug("==== onViewKeyEvent:key=%d, nativeVirtualKey=0x%02x, nativeScanCode=0x%02x, modifiers=0x%08x, "
    //            "nativeModifiers=0x%08x, text=%s",
    //            (Qt::Key)(event->key()),
    //            event->nativeVirtualKey(),
    //            event->nativeScanCode(),
    //            (quint32)(event->modifiers()),
    //            event->nativeModifiers(),
    //            event->text().toStdString().c_str());
    // #endif

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
  }
}

void
QCefViewPrivate::onViewMouseEvent(QMouseEvent* event)
{
  if (isOSRModeEnabled_) {
    // OSR mode
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
  }
}

void
QCefViewPrivate::onViewWheelEvent(QWheelEvent* event)
{
  if (isOSRModeEnabled_) {
    // OSR mode
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    auto p = event->pos();
#else
    auto p = event->position();
#endif

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

    // Prevent diagonal scrolling: only allow scrolling in one direction at a time
    if (qAbs(d.x()) > qAbs(d.y())) {
      d.setY(0);
    } else {
      d.setX(0);
    }

    pCefBrowser_->GetHost()->SendMouseWheelEvent(e, d.x(), d.y());
  }
}

void
QCefViewPrivate::onContextMenuEvent(const QPoint& pos)
{
  if (isOSRModeEnabled_) {
    // OSR mode
    if (osr.isShowingContextMenu_) {
      osr.contextMenu_->popup(pos);
    }
  }
}

void
QCefViewPrivate::onDragEnter(QDragEnterEvent* event)
{
  if (isOSRModeEnabled_ && pCefBrowser_ && pCefBrowser_->GetHost()) {
    osr.allowedDragOperations_ = 0;

    CefRefPtr<CefDragData> dragData = CreateCefDragDataFromQMimeData(*(event->mimeData()));

    CefMouseEvent e;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    auto b = event->mouseButtons();
    auto m = event->keyboardModifiers();
    e.x = event->pos().x();
    e.y = event->pos().y();
#else
    auto b = event->buttons();
    auto m = event->modifiers();
    e.x = event->position().x();
    e.y = event->position().y();
#endif
    e.modifiers |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;
    e.modifiers |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
    e.modifiers |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
    e.modifiers |= b & Qt::LeftButton ? EVENTFLAG_LEFT_MOUSE_BUTTON : 0;
    e.modifiers |= b & Qt::RightButton ? EVENTFLAG_RIGHT_MOUSE_BUTTON : 0;
    e.modifiers |= b & Qt::MiddleButton ? EVENTFLAG_MIDDLE_MOUSE_BUTTON : 0;

    auto proposedAction = event->proposedAction();
    CefBrowserHost::DragOperationsMask allowedOps = MapQDropActionToCefDragOperation(proposedAction);
    pCefBrowser_->GetHost()->DragTargetDragEnter(dragData, e, allowedOps);
    pCefBrowser_->GetHost()->DragTargetDragOver(e, allowedOps);

    event->setDropAction(static_cast<Qt::DropAction>(osr.allowedDragOperations_));
    event->accept();
  }
}

void
QCefViewPrivate::onDragMove(QDragMoveEvent* event)
{
  if (isOSRModeEnabled_ && pCefBrowser_ && pCefBrowser_->GetHost()) {
    CefMouseEvent e;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    auto b = event->mouseButtons();
    auto m = event->keyboardModifiers();
    e.x = event->pos().x();
    e.y = event->pos().y();
#else
    auto b = event->buttons();
    auto m = event->modifiers();
    e.x = event->position().x();
    e.y = event->position().y();
#endif
    e.modifiers |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;
    e.modifiers |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
    e.modifiers |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
    e.modifiers |= b & Qt::LeftButton ? EVENTFLAG_LEFT_MOUSE_BUTTON : 0;
    e.modifiers |= b & Qt::RightButton ? EVENTFLAG_RIGHT_MOUSE_BUTTON : 0;
    e.modifiers |= b & Qt::MiddleButton ? EVENTFLAG_MIDDLE_MOUSE_BUTTON : 0;

    auto proposedAction = event->proposedAction();
    CefBrowserHost::DragOperationsMask allowedOps = MapQDropActionToCefDragOperation(proposedAction);
    pCefBrowser_->GetHost()->DragTargetDragOver(e, allowedOps);

    event->setDropAction(static_cast<Qt::DropAction>(osr.allowedDragOperations_));
    event->accept();
  }
}

void
QCefViewPrivate::onDragLeave(QDragLeaveEvent* event)
{
  if (isOSRModeEnabled_ && pCefBrowser_ && pCefBrowser_->GetHost()) {
    osr.allowedDragOperations_ = 0;

    pCefBrowser_->GetHost()->DragTargetDragLeave();
  }
}

void
QCefViewPrivate::onDrop(QDropEvent* event)
{
  if (isOSRModeEnabled_ && pCefBrowser_ && pCefBrowser_->GetHost()) {

    CefMouseEvent e;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    auto b = event->mouseButtons();
    auto m = event->keyboardModifiers();
    e.x = event->pos().x();
    e.y = event->pos().y();
#else
    auto b = event->buttons();
    auto m = event->modifiers();
    e.x = event->position().x();
    e.y = event->position().y();
#endif
    e.modifiers |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;
    e.modifiers |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
    e.modifiers |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
    e.modifiers |= b & Qt::LeftButton ? EVENTFLAG_LEFT_MOUSE_BUTTON : 0;
    e.modifiers |= b & Qt::RightButton ? EVENTFLAG_RIGHT_MOUSE_BUTTON : 0;
    e.modifiers |= b & Qt::MiddleButton ? EVENTFLAG_MIDDLE_MOUSE_BUTTON : 0;

    pCefBrowser_->GetHost()->DragTargetDrop(e);

    event->acceptProposedAction();
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
  std::string data = content.toStdString();
  data = CefURIEncode(CefBase64Encode(data.c_str(), data.size()), false).ToString();
  data = "data:text/html;base64," + data;
  lastUrl_.FromString(data);

  if (pCefBrowser_) {
    pCefBrowser_->GetMainFrame()->LoadURL(lastUrl_);
  }
}

void
QCefViewPrivate::navigateToUrl(const QString& url)
{
  lastUrl_.FromString(url.toStdString());

  if (pCefBrowser_) {
    pCefBrowser_->GetMainFrame()->LoadURL(lastUrl_);
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
                              const QCefFrameId& frameId /*= QCefView::MainFrameID*/)
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
    query.markAsReplied();
    return pClient_->ResponseQuery(query.id(), query.result(), res, query.error());
  }
  return false;
}

bool
QCefViewPrivate::responseQCefQuery(const int64_t query, bool success, const QString& response, int error)
{
  if (pClient_) {
    CefString res;
    res.FromString(response.toStdString());
    return pClient_->ResponseQuery(query, success, res, error);
  }
  return false;
}

bool
QCefViewPrivate::executeJavascript(const QCefFrameId& frameId, const QString& code, const QString& url)
{
  if (code.isEmpty())
    return false;

  if (pCefBrowser_) {
    // convert frame id
    auto fid = ValueConvertor::FrameIdQ2C(frameId);
    // get frame instance
    auto frame = (frameId == QCefView::MainFrameID) ? //
                   pCefBrowser_->GetMainFrame()       // get main frame
                                                    : //
#if CEF_VERSION_MAJOR < 122                           // get frame by id
                   pCefBrowser_->GetFrame(fid);
#else
                   pCefBrowser_->GetFrameByIdentifier(fid);
#endif

    // validate frame instance
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

    frame->ExecuteJavaScript(c, u, 0);
    return true;
  }

  return false;
}

bool
QCefViewPrivate::executeJavascriptWithResult(const QCefFrameId& frameId,
                                             const QString& code,
                                             const QString& url,
                                             const QString& context)
{
  if (code.isEmpty())
    return false;

  if (pClient_ && pCefBrowser_) {
    // convert frame id
    auto fid = ValueConvertor::FrameIdQ2C(frameId);
    // get frame instance
    auto frame = (frameId == QCefView::MainFrameID) ? //
                   pCefBrowser_->GetMainFrame()       // get main frame
                                                    : //
#if CEF_VERSION_MAJOR < 122                           // get frame by id
                   pCefBrowser_->GetFrame(fid);
#else
                   pCefBrowser_->GetFrameByIdentifier(fid);
#endif

    // validate frame instance
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

    CefString ctx;
    ctx.FromString(context.toStdString());

    return pClient_->AsyncExecuteJSCode(pCefBrowser_, frame, c, u, ctx);
  }

  return false;
}

void
QCefViewPrivate::notifyMoveOrResizeStarted()
{
#if defined(Q_OS_WINDOWS) || defined(Q_OS_LINUX)
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->NotifyMoveOrResizeStarted();
    }
  }
#endif
}

bool
QCefViewPrivate::sendEventNotifyMessage(const QCefFrameId& frameId, const QString& name, const QVariantList& args)
{
  if (!pClient_) {
    return false;
  }

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

  return pClient_->TriggerEvent(pCefBrowser_, ValueConvertor::FrameIdQ2C(frameId), msg);
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

void
QCefViewPrivate::setOSRFrameRate(int fps)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->SetWindowlessFrameRate(fps);
    }
  }
}

void
QCefViewPrivate::setZoomLevel(double level)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->SetZoomLevel(level);
    }
  }
}

double
QCefViewPrivate::zoomLevel()
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      return host->GetZoomLevel();
    }
  }

  return 0;
}
