//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/menu.hpp"
#include "hyperion/platform/windows/windows_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class WindowsMenuType {
    Menu,
    ContextMenu
  };
  
  class WindowsMenu final {
  public:
    static HMENU CreateMenu(const Menu &menu, WindowsMenuType type);
    static void CallMenuItem(const Menu &menu, uint32 identifier);
  private:
    static HMENU CreateSubMenu(const Array<MenuItem> &items, WindowsMenuType type, uint32 &identifier_counter);
    static const MenuItem *FindMenuItem(const Array<MenuItem> &items, uint32 identifier, uint32 &identifier_counter);
  private:
    WindowsMenu() = delete;
    ~WindowsMenu() = delete;
  };
  
}
  