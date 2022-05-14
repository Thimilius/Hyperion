//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/main.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <format>

#include <rttr/registration.h>

using namespace rttr;

struct MyStruct {
  MyStruct() { };
  void func(double) { };

  int data;
  std::vector<int> vector;
};

RTTR_REGISTRATION
{
    registration::class_<MyStruct>("MyStruct")
      .constructor<>()
      .property("data", &MyStruct::data)
      .property("vector", &MyStruct::vector)
      .method("func", &MyStruct::func);
}

using Type = rttr::type;
using Variant = rttr::variant;
using Instance = rttr::instance;

Instance CreateInstanceFromType(void *data, Type type) {
  Instance i = data;
  (reinterpret_cast<rttr::detail::data_address_container *>(&i))->m_type = type;
  return i;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  uint32 Main::Run() {
    Engine::Setup();
    Engine::RegisterTypes();

    type t = type::get<MyStruct>();

    MyStruct s = { };
    s.data = 17;
    s.vector.push_back(13);
    MyStruct *p = &s;

    auto array_variant = t.get_property_value("vector", s);
    if (array_variant.is_sequential_container()) {
      auto view = array_variant.create_sequential_view();
      auto value = view.get_value(0);
      if (value.is_valid()) {
        auto type = value.get_type();
        auto name = type.get_name();
      }
    }

    void *void_p = p;

    instance i = CreateInstanceFromType(void_p, t);

    variant value = t.get_property_value("vector", i);
    if (value.is_valid()) {
      if (value.is_sequential_container()) {
        auto sequential = value.create_sequential_view();
        auto size = sequential.get_size();
        auto vvv = sequential.get_value(0);
        if (vvv.is_valid()) {
          auto array_value = vvv.get_wrapped_value<int>();
          auto foo = 1;
        }
      }
    }

    for (auto &prop : t.get_properties())
      std::cout << "name: " << prop.get_name() << std::endl;
    for (auto &meth : t.get_methods())
      std::cout << "name: " << meth.get_name() << std::endl;

    Application *application = Hyperion::CreateApplication();
    uint32 exit_code = Engine::Run();
    delete application;

    return exit_code;
  }

}
