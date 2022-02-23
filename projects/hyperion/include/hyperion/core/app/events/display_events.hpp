//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/events/app_event.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class DisplayChangeAppEvent : public AppEvent {
  public:
    AppEventType GetType() const override { return AppEventType::DisplayChange; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Display; }
  public:
    static AppEventType GetStaticType() { return AppEventType::DisplayChange; }
  };

}