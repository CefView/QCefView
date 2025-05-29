#pragma once

#pragma region qt_headers
#include <QMimeData>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_render_handler.h>
#pragma endregion

/// <summary>
/// Maps a CEF drag operation mask to the corresponding Qt drop actions.
/// </summary>
/// <param name="operation">The CEF drag operations mask to be mapped.</param>
/// <returns>
/// The Qt drop actions corresponding to the specified CEF drag operation mask.
/// </returns>
Qt::DropActions
MapCefDragOperationToQDropAction(CefRenderHandler::DragOperationsMask operation);

/// <summary>
/// Creates a QMimeData object from the given CefDragData instance.
/// </summary>
/// <param name="dragData">A reference to a CefDragData object containing the drag-and-drop data to convert.</param>
/// <returns>A pointer to a new QMimeData object representing the data from the provided CefDragData.</returns>
QMimeData*
CreateQMimeDataFromCefDragData(CefDragData& dragData);

/// <summary>
/// Maps Qt drop actions to the corresponding CEF drag operations mask.
/// </summary>
/// <param name="actions">
/// The Qt drop actions to be mapped, specified as a combination of Qt::DropActions flags.
/// </param>
/// <returns>
/// A CefRenderHandler::DragOperationsMask value representing the equivalent CEF drag operations for the given
/// Qt drop actions.
/// </returns>
CefRenderHandler::DragOperationsMask
MapQDropActionToCefDragOperation(Qt::DropActions actions);

/// <summary>
/// Creates a CefDragData object from a QMimeData instance.
/// </summary>
/// <param name="mime">A reference to the QMimeData object containing the data to be converted.</param>
/// <returns>A CefRefPtr to a CefDragData object representing the data from the provided QMimeData.</returns>
CefRefPtr<CefDragData>
CreateCefDragDataFromQMimeData(const QMimeData& mime);
