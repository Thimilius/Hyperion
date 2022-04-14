//------------------------ Header Guard ------------------------
#pragma once

// ────────────────────────────────────────────────────────────────────────────────────
// NOTE: Implementation taken from: https://github.com/skypjack/meta
// 
// The MIT License (MIT)
// 
// Copyright (c) 2018-2019 Michele Caini
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copy of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copy or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ────────────────────────────────────────────────────────────────────────────────────

//--------------- C++ Standard Library Includes ----------------
#include <tuple>
#include <array>
#include <cassert>
#include <cstddef>
#include <utility>
#include <functional>
#include <type_traits>

//---------------------- Project Includes ----------------------
#include "hyperion/core/meta/meta_policy.hpp"
#include "hyperion/core/meta/meta.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion { namespace Internal {

    template<typename>
    struct FunctionHelper;

    template<typename Ret, typename... Args>
    struct FunctionHelper<Ret(Args ...)> {
      using return_type = std::remove_cv_t<std::remove_reference_t<Ret>>;
      using args_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;

      static constexpr auto size = sizeof...(Args);
      static constexpr auto is_const = false;

      static auto arg(typename Internal::MetaFunctionNode::size_type index) {
        return std::array<MetaTypeNode *, sizeof...(Args)> { { MetaTypeInfo<Args>::Resolve()... } }[index];
      }
    };

    template<typename Ret, typename... Args>
    struct FunctionHelper<Ret(Args ...) const> : FunctionHelper<Ret(Args ...)> {
      static constexpr auto is_const = true;
    };

    template<typename Ret, typename... Args, typename Class>
    constexpr FunctionHelper<Ret(Args ...)>
    ToFunctionHelper(Ret (Class::*)(Args ...));

    template<typename Ret, typename... Args, typename Class>
    constexpr FunctionHelper<Ret(Args ...) const>
    ToFunctionHelper(Ret (Class::*)(Args ...) const);

    template<typename Ret, typename... Args>
    constexpr FunctionHelper<Ret(Args ...)>
    ToFunctionHelper(Ret (*)(Args ...));

    constexpr void ToFunctionHelper(...);

    template<typename Candidate>
    using FunctionHelperType = decltype(ToFunctionHelper(std::declval<Candidate>()));

    template<typename T, typename... Args, std::size_t... Indexes>
    Any Construct(Any *const args, std::index_sequence<Indexes...>) {
      [[maybe_unused]] auto direct = std::make_tuple((args + Indexes)->TryCast<Args>()...);
      Any Any { };

      if (((std::get<Indexes>(direct) || (args + Indexes)->Convert<Args>()) && ...)) {
        Any = T { (std::get<Indexes>(direct) ? *std::get<Indexes>(direct) : (args + Indexes)->Cast<Args>())... };
      }

      return Any;
    }

    template<bool8 Const, typename T, auto Data>
    bool8 Setter([[maybe_unused]] MetaHandle MetaHandle, [[maybe_unused]] Any index, [[maybe_unused]] Any value) {
      bool8 accepted = false;

      if constexpr (!Const) {
        if constexpr (std::is_function_v<std::remove_pointer_t<decltype(Data)>> || std::is_member_function_pointer_v<decltype(Data)>) {
          using helper_type = FunctionHelperType<decltype(Data)>;
          using data_type = std::tuple_element_t<!std::is_member_function_pointer_v<decltype(Data)>, typename helper_type::args_type>;
          static_assert(std::is_invocable_v<decltype(Data), T &, data_type>);
          auto *clazz = Any { MetaHandle }.TryCast<T>();
          auto *direct = value.TryCast<data_type>();

          if (clazz && (direct || value.Convert<data_type>())) {
            std::invoke(Data, *clazz, direct ? *direct : value.Cast<data_type>());
            accepted = true;
          }
        } else if constexpr (std::is_member_object_pointer_v<decltype(Data)>) {
          using data_type = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<T>().*Data)>>;
          static_assert(std::is_invocable_v<decltype(Data), T *>);
          auto *clazz = Any { MetaHandle }.TryCast<T>();

          if constexpr (std::is_array_v<data_type>) {
            using underlying_type = std::remove_extent_t<data_type>;
            auto *direct = value.TryCast<underlying_type>();
            auto *idx = index.TryCast<std::size_t>();

            if (clazz && idx && (direct || value.Convert<underlying_type>())) {
              std::invoke(Data, clazz)[*idx] = direct ? *direct : value.Cast<underlying_type>();
              accepted = true;
            }
          } else {
            auto *direct = value.TryCast<data_type>();

            if (clazz && (direct || value.Convert<data_type>())) {
              std::invoke(Data, clazz) = (direct ? *direct : value.Cast<data_type>());
              accepted = true;
            }
          }
        } else {
          static_assert(std::is_pointer_v<decltype(Data)>);
          using data_type = std::remove_cv_t<std::remove_reference_t<decltype(*Data)>>;

          if constexpr (std::is_array_v<data_type>) {
            using underlying_type = std::remove_extent_t<data_type>;
            auto *direct = value.TryCast<underlying_type>();
            auto *idx = index.TryCast<std::size_t>();

            if (idx && (direct || value.Convert<underlying_type>())) {
              (*Data)[*idx] = (direct ? *direct : value.Cast<underlying_type>());
              accepted = true;
            }
          } else {
            auto *direct = value.TryCast<data_type>();

            if (direct || value.Convert<data_type>()) {
              *Data = (direct ? *direct : value.Cast<data_type>());
              accepted = true;
            }
          }
        }
      }

      return accepted;
    }

    template<typename T, auto Data, typename Policy>
    Any Getter([[maybe_unused]] MetaHandle MetaHandle, [[maybe_unused]] Any index) {
      auto dispatch = [](auto &&value) {
        if constexpr (std::is_same_v<Policy, MetaPolicyAsVoid>) {
          return Any { std::in_place_type<void> };
        } else if constexpr (std::is_same_v<Policy, MetaPolicyAsAlias>) {
          return Any { std::ref(std::forward<decltype(value)>(value)) };
        } else {
          static_assert(std::is_same_v<Policy, MetaPolicyAsIs>);
          return Any { std::forward<decltype(value)>(value) };
        }
      };

      if constexpr (std::is_function_v<std::remove_pointer_t<decltype(Data)>> || std::is_member_function_pointer_v<decltype(Data)>) {
        static_assert(std::is_invocable_v<decltype(Data), T &>);
        auto *clazz = Any { MetaHandle }.TryCast<T>();
        return clazz ? dispatch(std::invoke(Data, *clazz)) : Any { };
      } else if constexpr (std::is_member_object_pointer_v<decltype(Data)>) {
        using data_type = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<T>().*Data)>>;
        static_assert(std::is_invocable_v<decltype(Data), T *>);
        auto *clazz = Any { MetaHandle }.TryCast<T>();

        if constexpr (std::is_array_v<data_type>) {
          auto *idx = index.TryCast<std::size_t>();
          return (clazz && idx) ? dispatch(std::invoke(Data, clazz)[*idx]) : Any { };
        } else {
          return clazz ? dispatch(std::invoke(Data, clazz)) : Any { };
        }
      } else {
        static_assert(std::is_pointer_v<std::decay_t<decltype(Data)>>);

        if constexpr (std::is_array_v<std::remove_pointer_t<decltype(Data)>>) {
          auto *idx = index.TryCast<std::size_t>();
          return idx ? dispatch((*Data)[*idx]) : Any { };
        } else {
          return dispatch(*Data);
        }
      }
    }

    template<typename T, auto Candidate, typename Policy, std::size_t... Indexes>
    Any Invoke([[maybe_unused]] MetaHandle MetaHandle, Any *args, std::index_sequence<Indexes...>) {
      using helper_type = FunctionHelperType<decltype(Candidate)>;

      auto dispatch = [](auto *... args) {
        if constexpr (std::is_void_v<typename helper_type::return_type> || std::is_same_v<Policy, MetaPolicyAsVoid>) {
          std::invoke(Candidate, *args...);
          return Any { std::in_place_type<void> };
        } else if constexpr (std::is_same_v<Policy, MetaPolicyAsAlias>) {
          return Any { std::ref(std::invoke(Candidate, *args...)) };
        } else {
          static_assert(std::is_same_v<Policy, MetaPolicyAsIs>);
          return Any { std::invoke(Candidate, *args...) };
        }
      };

      [[maybe_unused]] const auto direct = std::make_tuple([](Hyperion::Any *Any, auto *instance) {
        using arg_type = std::remove_reference_t<decltype(*instance)>;

        if (!instance && Any->Convert<arg_type>()) {
          instance = Any->TryCast<arg_type>();
        }

        return instance;
      }(args + Indexes, (args + Indexes)->TryCast<std::tuple_element_t<Indexes, typename helper_type::args_type>>())...);

      if constexpr (std::is_function_v<std::remove_pointer_t<decltype(Candidate)>>) {
        return (std::get<Indexes>(direct) && ...) ? dispatch(std::get<Indexes>(direct)...) : Any { };
      } else {
        auto *clazz = Any { MetaHandle }.TryCast<T>();
        return (clazz && (std::get<Indexes>(direct) && ...)) ? dispatch(clazz, std::get<Indexes>(direct)...) : Any { };
      }
    }

  }

  template<typename T>
  class MetaFactory {
  public:
    MetaFactory() = default;
  public:
    template<typename... Attribute>
    MetaFactory Type(const String &name, Attribute &&... attribute) {
      return Type(name, MetaPrimitiveType::None, attribute ...);
    }

    template<typename Base>
    MetaFactory Base() {
      static_assert(std::is_base_of_v<Base, T>);
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaBaseNode node {
        &Internal::MetaTypeInfo<T>::template base<Base>,
        type,
        nullptr,
        &Internal::MetaTypeInfo<Base>::Resolve,
        [](void *instance) -> void* {
          return static_cast<Base *>(static_cast<T *>(instance));
        },
        []() -> Hyperion::MetaBase {
          return &node;
        }
      };

      node.next = type->base;
      assert((!Internal::MetaTypeInfo<T>::template base<Base>));
      Internal::MetaTypeInfo<T>::template base<Base> = &node;
      type->base = &node;

      return *this;
    }

    template<typename To>
    MetaFactory Conversion() {
      static_assert(std::is_convertible_v<T, To>);
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaConversionNode node {
        &Internal::MetaTypeInfo<T>::template conv<To>,
        type,
        nullptr,
        &Internal::MetaTypeInfo<To>::Resolve,
        [](const void *instance) -> Any {
          return static_cast<To>(*static_cast<const T *>(instance));
        },
        []() -> Hyperion::MetaConversion {
          return &node;
        }
      };

      node.next = type->conv;
      assert((!Internal::MetaTypeInfo<T>::template conv<To>));
      Internal::MetaTypeInfo<T>::template conv<To> = &node;
      type->conv = &node;

      return *this;
    }

    template<auto Candidate>
    MetaFactory Conversion() {
      using conv_type = std::invoke_result_t<decltype(Candidate), T &>;
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaConversionNode node {
        &Internal::MetaTypeInfo<T>::template conv<conv_type>,
        type,
        nullptr,
        &Internal::MetaTypeInfo<conv_type>::Resolve,
        [](const void *instance) -> Any {
          return std::invoke(Candidate, *static_cast<const T *>(instance));
        },
        []() -> Hyperion::MetaConversion {
          return &node;
        }
      };

      node.next = type->conv;
      assert((!Internal::MetaTypeInfo<T>::template conv<conv_type>));
      Internal::MetaTypeInfo<T>::template conv<conv_type> = &node;
      type->conv = &node;

      return *this;
    }

    template<auto Func, typename Policy = MetaPolicyAsIs, typename... Attribute>
    MetaFactory Constructor(Attribute &&... attribute) {
      using helper_type = Internal::FunctionHelperType<decltype(Func)>;
      static_assert(std::is_same_v<typename helper_type::return_type, T>);
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaConstructorNode node {
        &Internal::MetaTypeInfo<T>::template ctor<typename helper_type::args_type>,
        type,
        nullptr,
        nullptr,
        helper_type::size,
        &helper_type::arg,
        [](Any *const Any) {
          return Internal::Invoke<T, Func, Policy>({ }, Any, std::make_index_sequence<helper_type::size> { });
        },
        []() -> Hyperion::MetaConstructor {
          return &node;
        }
      };

      node.next = type->ctor;
      node.attribute = Attributes<typename helper_type::args_type>(std::forward<Attribute>(attribute)...);
      assert((!Internal::MetaTypeInfo<T>::template ctor<typename helper_type::args_type>));
      Internal::MetaTypeInfo<T>::template ctor<typename helper_type::args_type> = &node;
      type->ctor = &node;

      return *this;
    }

    template<typename... Args, typename... Attribute>
    MetaFactory Constructor(Attribute &&... attribute) {
      using helper_type = Internal::FunctionHelperType<T(*)(Args ...)>;
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaConstructorNode node {
        &Internal::MetaTypeInfo<T>::template ctor<typename helper_type::args_type>,
        type,
        nullptr,
        nullptr,
        helper_type::size,
        &helper_type::arg,
        [](Any *const Any) {
          return Internal::Construct<T, std::remove_cv_t<std::remove_reference_t<Args>>...>(Any, std::make_index_sequence<helper_type::size> { });
        },
        []() -> Hyperion::MetaConstructor {
          return &node;
        }
      };

      node.next = type->ctor;
      node.attribute = Attributes<typename helper_type::args_type>(std::forward<Attribute>(attribute)...);
      assert((!Internal::MetaTypeInfo<T>::template ctor<typename helper_type::args_type>));
      Internal::MetaTypeInfo<T>::template ctor<typename helper_type::args_type> = &node;
      type->ctor = &node;

      return *this;
    }

    template<auto Func>
    MetaFactory Destructor() {
      static_assert(std::is_invocable_v<decltype(Func), T &>);
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaDestructorNode node {
        &Internal::MetaTypeInfo<T>::template dtor<Func>,
        type,
        [](MetaHandle MetaHandle) {
          const auto valid = (MetaHandle.type() == Internal::MetaTypeInfo<T>::Resolve()->clazz());

          if (valid) {
            std::invoke(Func, *Any { MetaHandle }.TryCast<T>());
          }

          return valid;
        },
        []() -> Hyperion::MetaDestructor {
          return &node;
        }
      };

      assert(!Internal::MetaTypeInfo<T>::type->dtor);
      assert((!Internal::MetaTypeInfo<T>::template dtor<Func>));
      Internal::MetaTypeInfo<T>::template dtor<Func> = &node;
      Internal::MetaTypeInfo<T>::type->dtor = &node;

      return *this;
    }

    template<auto Data, typename Policy = MetaPolicyAsIs, typename... Attribute>
    MetaFactory Property(const String &name, Attribute &&... attribute) {
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();
      Internal::MetaPropertyNode *curr = nullptr;

      if constexpr (std::is_same_v<T, decltype(Data)>) {
        static_assert(std::is_same_v<Policy, MetaPolicyAsIs>);

        static Internal::MetaPropertyNode node {
          &Internal::MetaTypeInfo<T>::template data<Data>,
          { },
          { },
          type,
          nullptr,
          nullptr,
          true,
          true,
          &Internal::MetaTypeInfo<T>::Resolve,
          [](MetaHandle, Any, Any) { return false; },
          [](MetaHandle, Any) -> Any { return Data; },
          []() -> Hyperion::MetaProperty {
            return &node;
          }
        };

        node.attribute = Attributes<std::integral_constant<T, Data>>(std::forward<Attribute>(attribute)...);
        curr = &node;
      } else if constexpr (std::is_member_object_pointer_v<decltype(Data)>) {
        using data_type = std::remove_reference_t<decltype(std::declval<T>().*Data)>;

        static Internal::MetaPropertyNode node {
          &Internal::MetaTypeInfo<T>::template data<Data>,
          { },
          { },
          type,
          nullptr,
          nullptr,
          std::is_const_v<data_type>,
          !std::is_member_object_pointer_v<decltype(Data)>,
          &Internal::MetaTypeInfo<data_type>::Resolve,
          &Internal::Setter<std::is_const_v<data_type>, T, Data>,
          &Internal::Getter<T, Data, Policy>,
          []() -> Hyperion::MetaProperty {
            return &node;
          }
        };

        node.attribute = Attributes<std::integral_constant<decltype(Data), Data>>(std::forward<Attribute>(attribute)...);
        curr = &node;
      } else {
        static_assert(std::is_pointer_v<std::decay_t<decltype(Data)>>);
        using data_type = std::remove_pointer_t<std::decay_t<decltype(Data)>>;

        static Internal::MetaPropertyNode node {
          &Internal::MetaTypeInfo<T>::template data<Data>,
          { },
          { },
          type,
          nullptr,
          nullptr,
          std::is_const_v<data_type>,
          !std::is_member_object_pointer_v<decltype(Data)>,
          &Internal::MetaTypeInfo<data_type>::Resolve,
          &Internal::Setter<std::is_const_v<data_type>, T, Data>,
          &Internal::Getter<T, Data, Policy>,
          []() -> Hyperion::MetaProperty {
            return &node;
          }
        };

        node.attribute = Attributes<std::integral_constant<decltype(Data), Data>>(std::forward<Attribute>(attribute)...);
        curr = &node;
      }

      curr->identifier = std::hash<String> { }(name);
      curr->name = name;
      curr->next = type->data;
      assert(!Duplicate(curr->identifier, curr->next));
      assert((!Internal::MetaTypeInfo<T>::template data<Data>));
      Internal::MetaTypeInfo<T>::template data<Data> = curr;
      type->data = curr;

      return *this;
    }

    template<auto Setter, auto Getter, typename Policy = MetaPolicyAsIs, typename... Attribute>
    MetaFactory Property(const String &name, Attribute &&... attribute) {
      using owner_type = std::tuple<std::integral_constant<decltype(Setter), Setter>, std::integral_constant<decltype(Getter), Getter>>;
      using underlying_type = std::invoke_result_t<decltype(Getter), T &>;
      static_assert(std::is_invocable_v<decltype(Setter), T &, underlying_type>);
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaPropertyNode node {
        &Internal::MetaTypeInfo<T>::template data<Setter, Getter>,
        { },
        { },
        type,
        nullptr,
        nullptr,
        false,
        false,
        &Internal::MetaTypeInfo<underlying_type>::Resolve,
        &Internal::Setter<false, T, Setter>,
        &Internal::Getter<T, Getter, Policy>,
        []() -> Hyperion::MetaProperty {
          return &node;
        }
      };

      node.identifier = std::hash<String> { }(name);
      node.name = name;
      node.next = type->data;
      node.attribute = Attributes<owner_type>(std::forward<Attribute>(attribute)...);
      assert(!Duplicate(node.identifier, node.next));
      assert((!Internal::MetaTypeInfo<T>::template data<Setter, Getter>));
      Internal::MetaTypeInfo<T>::template data<Setter, Getter> = &node;
      type->data = &node;

      return *this;
    }

    template<auto Candidate, typename Policy = MetaPolicyAsIs, typename... Attribute>
    MetaFactory Function(const String &name, Attribute &&... attribute) {
      using owner_type = std::integral_constant<decltype(Candidate), Candidate>;
      using helper_type = Internal::FunctionHelperType<decltype(Candidate)>;
      auto *const type = Internal::MetaTypeInfo<T>::Resolve();

      static Internal::MetaFunctionNode node {
        &Internal::MetaTypeInfo<T>::template func<Candidate>,
        { },
        { },
        type,
        nullptr,
        nullptr,
        helper_type::size,
        helper_type::is_const,
        !std::is_member_function_pointer_v<decltype(Candidate)>,
        &Internal::MetaTypeInfo<std::conditional_t<std::is_same_v<Policy, MetaPolicyAsVoid>, void, typename helper_type::return_type>>::Resolve,
        &helper_type::arg,
        [](MetaHandle MetaHandle, Any *Any) {
          return Internal::Invoke<T, Candidate, Policy>(MetaHandle, Any, std::make_index_sequence<helper_type::size> { });
        },
        []() -> Hyperion::MetaFunction {
          return &node;
        }
      };

      node.identifier = std::hash<String> { }(name);
      node.name = name;
      node.next = type->func;
      node.attribute = Attributes<owner_type>(std::forward<Attribute>(attribute)...);
      assert(!Duplicate(node.identifier, node.next));
      assert((!Internal::MetaTypeInfo<T>::template func<Candidate>));
      Internal::MetaTypeInfo<T>::template func<Candidate> = &node;
      type->func = &node;

      return *this;
    }

    bool8 Unregister() {
      const auto registered = Internal::MetaTypeInfo<T>::type;

      if (registered) {
        if (auto *curr = Internal::MetaTypeInfo<>::type; curr == Internal::MetaTypeInfo<T>::type) {
          Internal::MetaTypeInfo<>::type = Internal::MetaTypeInfo<T>::type->next;
        } else {
          while (curr && curr->next != Internal::MetaTypeInfo<T>::type) {
            curr = curr->next;
          }

          if (curr) {
            curr->next = Internal::MetaTypeInfo<T>::type->next;
          }
        }

        UnregisterProperty(&Internal::MetaTypeInfo<T>::type->attribute);
        UnregisterAll<&Internal::MetaTypeNode::base>(0);
        UnregisterAll<&Internal::MetaTypeNode::conv>(0);
        UnregisterAll<&Internal::MetaTypeNode::ctor>(0);
        UnregisterAll<&Internal::MetaTypeNode::data>(0);
        UnregisterAll<&Internal::MetaTypeNode::func>(0);
        UnregisterDestructor();

        Internal::MetaTypeInfo<T>::type->identifier = { };
        Internal::MetaTypeInfo<T>::type->next = nullptr;
        Internal::MetaTypeInfo<T>::type = nullptr;
      }

      return registered;
    }

  private:
    template<typename... Attribute>
    MetaFactory Type(const String &name, MetaPrimitiveType primitive_type, Attribute &&... attribute) {
      assert(!Internal::MetaTypeInfo<T>::type);
      auto *node = Internal::MetaTypeInfo<T>::Resolve();
      node->identifier = std::hash<String> { }(name);
      node->name = name;
      node->primitive_type = primitive_type;
      node->trivial_destructor = [](const void *instance) { static_cast<const T *>(instance)->~T(); };
      node->next = Internal::MetaTypeInfo<>::type;
      node->attribute = Attributes<T>(std::forward<Attribute>(attribute)...);
      assert(!Duplicate(node->identifier, node->next));
      Internal::MetaTypeInfo<T>::type = node;
      Internal::MetaTypeInfo<>::type = node;

      if constexpr (std::is_default_constructible_v<T>) {
        node->in_place_constructor = [](void *address) { return static_cast<void *>(new(address) T()); };
      }

      return *this;
    }

    template<typename Node>
    bool8 Duplicate(const std::size_t identifier, const Node *node) {
      return node && (node->identifier == identifier || Duplicate(identifier, node->next));
    }

    bool8 Duplicate(const Any &key, const Internal::MetaAttributeNode *node) {
      return node && (node->key() == key || Duplicate(key, node->next));
    }

    template<typename>
    Internal::MetaAttributeNode *Attributes() {
      return nullptr;
    }

    template<typename Owner, typename Attribute, typename... Other>
    Internal::MetaAttributeNode *Attributes(Attribute &&attribute, Other &&... other) {
      static std::remove_cv_t<std::remove_reference_t<Attribute>> attrib { };

      static Internal::MetaAttributeNode node {
        nullptr,
        []() -> Any {
          return std::as_const(std::get<0>(attrib));
        },
        []() -> Any {
          return std::as_const(std::get<1>(attrib));
        },
        []() -> Hyperion::MetaAttribute {
          return &node;
        }
      };

      attrib = std::forward<Attribute>(attribute);
      node.next = Attributes<Owner>(std::forward<Other>(other)...);
      assert(!Duplicate(Any{ std::get<0>(attribute) }, node.next));
      return &node;
    }

    void UnregisterProperty(Internal::MetaAttributeNode **attribute) {
      while (*attribute) {
        auto *node = *attribute;
        *attribute = node->next;
        node->next = nullptr;
      }
    }

    void UnregisterDestructor() {
      if (auto node = Internal::MetaTypeInfo<T>::type->dtor; node) {
        Internal::MetaTypeInfo<T>::type->dtor = nullptr;
        *node->underlying = nullptr;
      }
    }

    template<auto Member>
    auto UnregisterAll(int) -> decltype((Internal::MetaTypeInfo<T>::type->*Member)->attribute, void()) {
      while (Internal::MetaTypeInfo<T>::type->*Member) {
        auto node = Internal::MetaTypeInfo<T>::type->*Member;
        Internal::MetaTypeInfo<T>::type->*Member = node->next;
        UnregisterProperty(&node->attribute);
        node->next = nullptr;
        *node->underlying = nullptr;
      }
    }

    template<auto Member>
    void UnregisterAll(char) {
      while (Internal::MetaTypeInfo<T>::type->*Member) {
        auto node = Internal::MetaTypeInfo<T>::type->*Member;
        Internal::MetaTypeInfo<T>::type->*Member = node->next;
        node->next = nullptr;
        *node->underlying = nullptr;
      }
    }

  private:
    friend class Hyperion::Engine;
  };

  class MetaRegistry final {
  public:
    template<typename T, typename... Attribute>
    inline static MetaFactory<T> Reflect(const String &name, Attribute &&... attribute) {
      return MetaFactory<T> { }.Type(name, std::forward<Attribute>(attribute)...);
    }

    template<typename T>
    inline static MetaFactory<T> Reflect() { return MetaFactory<T> { }; }

    template<typename T>
    inline static bool8 Unregister() { return MetaFactory<T> { }.Unregister(); }

    template<typename T>
    inline static MetaType Resolve() { return Internal::MetaTypeInfo<T>::Resolve()->clazz(); }

    inline static MetaType Resolve(const String &name) {
      const std::size_t identifier = std::hash<String> { }(name);
      const auto *curr = Internal::FindIf([identifier](auto *node) {
        return node->identifier == identifier;
      }, Internal::MetaTypeInfo<>::type);

      return curr ? curr->clazz() : MetaType { };
    }

    template<typename Op>
    inline static std::enable_if_t<std::is_invocable_v<Op, MetaType>, void>
    Resolve(Op op) {
      Internal::Iterate([op = std::move(op)](auto *node) {
        op(node->clazz());
      }, Internal::MetaTypeInfo<>::type);
    }
  };

}
