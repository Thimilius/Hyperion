//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/app/windows_menu.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  HMENU WindowsMenu::CreateMenu(const Menu &menu, WindowsMenuType type) {
    if (menu.items.IsEmpty()) {
      return nullptr;
    }
    
    uint32 identifier_counter = 1;
    return CreateSubMenu(menu.items, type, identifier_counter);
  }

  //--------------------------------------------------------------
  void WindowsMenu::CallMenuItem(const Menu &menu, uint32 identifier) {
    // We need to find the menu item the identifier belongs to and invoke the callback on that.
    if (identifier < 1) {
      return;
    }
    
    uint32 identifier_count = 1;
    const MenuItem *item = FindMenuItem(menu.items, identifier, identifier_count);
    if (item && item->callback) {
      item->callback(*item);
    }
  }
  
  //--------------------------------------------------------------
  HMENU WindowsMenu::CreateSubMenu(const Array<MenuItem> &items, WindowsMenuType type, uint32 &identifier_counter) {
    HMENU sub_menu = type == WindowsMenuType::Menu ? ::CreateMenu() : CreatePopupMenu();

    for (const MenuItem &item : items) {
      WideString wide_text = StringUtils::Utf8ToUtf16(item.text);

      MENUITEMINFOW item_info = { };
      item_info.cbSize = sizeof(MENUITEMINFOW);
      item_info.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING | MIIM_STATE;
      item_info.fType = MFT_STRING;
      item_info.fState = MFS_ENABLED | MFS_UNHILITE;
      item_info.wID = identifier_counter++;
      item_info.dwTypeData = wide_text.data();

      bool8 is_sub_menu = !item.sub_items.IsEmpty();
      HMENU sub_sub_menu = nullptr;
      if (is_sub_menu) {
        sub_sub_menu = CreateSubMenu(item.sub_items, type, identifier_counter);
        item_info.fMask |= MIIM_SUBMENU;
        item_info.hSubMenu = sub_sub_menu;
      }

      InsertMenuItemW(sub_menu, -1, true, &item_info);
    }

    return sub_menu;
  }
  
  //--------------------------------------------------------------
  const MenuItem *WindowsMenu::FindMenuItem(const Array<MenuItem> &items, uint32 identifier, uint32 &identifier_counter) {
    for (const MenuItem &item : items) {
      if (identifier_counter == identifier) {
        return &item;
      }
      identifier_counter++;

      const MenuItem *found_item = FindMenuItem(item.sub_items, identifier, identifier_counter);
      if (found_item) {
        return found_item;
      }
    }
    return nullptr;
  }

}
  