#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "../utils/CommonUtils.h"
#include "details/QCefViewPrivate.h"

bool
CCefClientDelegate::onDragEnter(CefRefPtr<CefBrowser>& browser,
                                CefRefPtr<CefDragData>& dragData,
                                CefDragHandler::DragOperationsMask mask)
{
  FLog();

  AcquireAndValidateCefViewPrivateWithReturn(pCefViewPrivate, true);

  bool allowDrop = false;
  runInMainThreadAndWait([&]() { allowDrop = pCefViewPrivate->shouldAllowDrop(dragData, mask); });

  // return true to cancel, false to allow
  return !allowDrop;
}

void
CCefClientDelegate::draggableRegionChanged(CefRefPtr<CefBrowser>& browser,
                                           CefRefPtr<CefFrame>& frame,
                                           const std::vector<CefDraggableRegion>& regions)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // Determine new draggable region.
  QRegion draggableRegion;
  QRegion nonDraggableRegion;
  std::vector<CefDraggableRegion>::const_iterator it = regions.begin();
  for (; it != regions.end(); ++it) {
    if (it->draggable) {
      draggableRegion += QRegion(it->bounds.x, it->bounds.y, it->bounds.width, it->bounds.height);
    } else {
      nonDraggableRegion += QRegion(it->bounds.x, it->bounds.y, it->bounds.width, it->bounds.height);
    }
  }

  runInMainThread([pCefViewPrivate, draggableRegion, nonDraggableRegion]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->draggableRegionChanged(draggableRegion, nonDraggableRegion);
    }
  });
}
