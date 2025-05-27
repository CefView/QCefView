#pragma once

#pragma region qt_headers
#include <QList>
#include <QMenu>
#include <QString>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion

namespace MenuBuilder {

/// <summary>
/// Represents the type of a menu item.
/// </summary>
typedef enum MenuItemType
{
  /// No menu item type specified.
  kMeueItemTypeNone,
  /// A standard command item (clickable action).
  kMeueItemTypeCommand,
  /// A checkable menu item (can be checked/unchecked).
  kMeueItemTypeCheck,
  /// A radio button menu item (part of a group, only one can be selected).
  kMeueItemTypeRadio,
  /// A separator (non-interactive visual divider).
  kMeueItemTypeSeparator,
  /// A submenu (contains nested menu items).
  kMeueItemTypeSubMenu,
} MenuItemType;

/// <summary>
/// Represents a single menu item, including its type, label, state, and
/// optional submenu data.
/// </summary>
typedef struct MenuItem
{
  /// The type of the menu item (command, check, radio, etc.).
  MenuItemType type = kMeueItemTypeNone;
  /// The display label for the menu item.
  QString label;
  /// The command identifier associated with this item.
  int commandId = 0;
  /// Whether the menu item is enabled (can be interacted with).
  bool enable = false;
  /// Whether the menu item is visible.
  bool visible = false;
  /// Whether the menu item is checked (for check/radio types).
  bool checked = false;
  /// The group identifier (used for radio groups).
  int groupId = -1;
  /// The accelerator key (if any) associated with this item.
  int accelerator = -1;
  /// Submenu data, if this item is a submenu.
  QList<MenuItem> subMenuData;
} MenuItem;

/// <summary>
/// A list of MenuItem objects representing a menu structure.
/// </summary>
typedef QList<MenuItem> MenuData;

/// <summary>
/// Creates a MenuData structure from a CefMenuModel.
/// Iterates through the CEF menu model and converts each item into a MenuItem,
/// preserving hierarchy for submenus.
/// </summary>
/// <param name="model">Pointer to the CefMenuModel to convert.</param>
/// <returns>A MenuData list representing the menu structure.</returns>
MenuData
CreateMenuDataFromCefMenu(CefMenuModel* model);

/// <summary>
/// Builds a QMenu from the provided MenuData structure.
/// Recursively adds actions and submenus to the given QMenu based on the
/// MenuData items.
/// </summary>
/// <param name="menu">Pointer to the QMenu to populate.</param>
/// <param name="data">MenuData structure describing the menu items.</param>
void
BuildQtMenuFromMenuData(QMenu* menu, const MenuData& data);

}; // namespace MenuBuilder
