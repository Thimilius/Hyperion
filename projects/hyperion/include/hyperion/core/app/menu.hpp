//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct MenuItem;

  using MenuItemCallback = std::function<void(const MenuItem &)>;

  struct MenuItem {
    String text;
    MenuItemCallback callback;
    Array<MenuItem> sub_items;
  };

  struct Menu {
    Array<MenuItem> items;
  };

}