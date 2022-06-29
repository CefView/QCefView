#include "MenuBuilder.h"

#include <QActionGroup>

namespace MenuBuilder {
MenuData
CreateMenuDataFromCefMenu(CefMenuModel* model)
{
  MenuData data;

  if (!model)
    return data;

  for (int i = 0; i < model->GetCount(); i++) {
    MenuItem item;
    auto type = model->GetTypeAt(i);
    item.type = (MenuItemType)(type);
    item.label = QString::fromUtf8(model->GetLabelAt(i).ToString().c_str());
    item.commandId = model->GetCommandIdAt(i);
    item.enable = model->IsEnabledAt(i);
    item.visible = model->IsVisibleAt(i);

    int keyCode = 0;
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    auto hasAccelerator = model->GetAcceleratorAt(i, keyCode, shift, ctrl, alt);
    if (hasAccelerator) {
      int combination = keyCode;
      if (shift)
        combination += Qt::SHIFT;
      if (ctrl)
        combination += Qt::CTRL;
      if (alt)
        combination += Qt::ALT;
      item.accelerator = combination;
    }

    switch (type) {
      case MENUITEMTYPE_COMMAND: {
      } break;
      case MENUITEMTYPE_CHECK: {
        item.checked = model->IsCheckedAt(i);
        item.groupId = model->GetGroupIdAt(i);
      } break;
      case MENUITEMTYPE_RADIO: {
        item.checked = model->IsCheckedAt(i);
        item.groupId = model->GetGroupIdAt(i);
      } break;
      case MENUITEMTYPE_SEPARATOR: {
      } break;
      case MENUITEMTYPE_SUBMENU: {
        auto cefSubMenu = model->GetSubMenuAt(i);
        item.subMenuData = CreateMenuDataFromCefMenu(cefSubMenu.get());
      } break;
      default:
        break;
    }

    data.append(item);
  }

  return data;
}

void
BuildQtMenuFromMenuData(QMenu* menu, const MenuData& data)
{
  if (!menu || data.isEmpty())
    return;

  QMap<int, QActionGroup*> groupMap;

  for (const auto& item : data) {
    switch (item.type) {
      case kMeueItemTypeCommand: {
        auto action = new QAction(item.label);
        action->setData(QVariant(item.commandId));
        action->setEnabled(item.enable);
        action->setVisible(item.visible);

        if (item.accelerator)
          action->setShortcut(QKeySequence(item.accelerator));

        menu->addAction(action);
      } break;
      case kMeueItemTypeCheck: {
        auto* action = new QAction(item.label);
        action->setData(QVariant(item.commandId));
        action->setEnabled(item.enable);
        action->setVisible(item.visible);

        action->setCheckable(true);
        action->setChecked(item.checked);

        menu->addAction(action);
      } break;
      case kMeueItemTypeRadio: {
        auto* action = new QAction(item.label);
        action->setData(QVariant(item.commandId));
        action->setEnabled(item.enable);
        action->setVisible(item.visible);

        action->setCheckable(true);
        action->setChecked(item.checked);

        if (item.groupId >= 0) {
          if (!groupMap.contains(item.groupId))
            groupMap[item.groupId] = new QActionGroup(menu);

          action->setActionGroup(groupMap[item.groupId]);
        }

        menu->addAction(action);
      } break;
      case kMeueItemTypeSeparator: {
        menu->addSeparator();
      } break;
      case kMeueItemTypeSubMenu: {
        if (!item.subMenuData.isEmpty()) {
          auto subMenu = menu->addMenu(item.label);
          BuildQtMenuFromMenuData(subMenu, item.subMenuData);

          subMenu->setEnabled(item.enable);
          subMenu->setVisible(item.visible);
        }
      } break;
      default:
        break;
    }
  }
}
} // namespace MenuBuilder
