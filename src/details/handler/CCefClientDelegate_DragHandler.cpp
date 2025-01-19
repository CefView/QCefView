#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "details/QCefViewPrivate.h"

bool
CCefClientDelegate::onDragEnter(CefRefPtr<CefBrowser>& browser,
                                CefRefPtr<CefDragData>& dragData,
                                CefDragHandler::DragOperationsMask mask)
{
  // Return false for default drag handling behavior or true to cancel the drag event.
  return !pCefViewPrivate_->q_ptr->isDragAndDropEnabled();
}

void
CCefClientDelegate::draggableRegionChanged(CefRefPtr<CefBrowser>& browser,
                                           CefRefPtr<CefFrame>& frame,
                                           const std::vector<CefDraggableRegion>& regions)
{
  if (!IsValidBrowser(browser))
    return;

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

  emit pCefViewPrivate_->q_ptr->draggableRegionChanged(draggableRegion, nonDraggableRegion);
}
