#pragma once

#include <QMenu>

#include <include/cef_app.h>

namespace MenuBuilder {
/// <summary>
///
/// </summary>
typedef enum MenuItemType
{
  kMeueItemTypeNone,
  kMeueItemTypeCommand,
  kMeueItemTypeCheck,
  kMeueItemTypeRadio,
  kMeueItemTypeSeparator,
  kMeueItemTypeSubMenu,
} MenuItemType;

/// <summary>
///
/// </summary>
typedef struct MenuItem
{
  MenuItemType type = kMeueItemTypeNone;
  QString label;
  int commandId = 0;
  bool enable = false;
  bool visible = false;
  bool checked = false;
  int groupId = -1;
  int accelerator = -1;
  QList<MenuItem> subMenuData;
} MenuItem;

typedef QList<MenuItem> MenuData;

MenuData
CreateMenuDataFromCefMenu(CefMenuModel* model);

void
BuildQtMenuFromMenuData(QMenu* menu, const MenuData& data);
}; // namespace MenuBuilder
