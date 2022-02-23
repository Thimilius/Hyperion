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
#include <array>
#include <memory>
#include <cstring>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <cassert>

//---------------------- Project Includes ----------------------
#include "hyperion/core/bit.hpp"
#include "hyperion/core/enum.hpp"
#include "hyperion/core/string_utils.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Any;
  class MetaHandle;
  class MetaAttribute;
  class MetaBase;
  class MetaConversion;
  class MetaConstructor;
  class MetaDestructor;
  class MetaProperty;
  class MetaFunction;
  class MetaType;

  namespace Internal {
    struct MetaTypeNode;
  }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class MetaTypeTraits {
    None = 0,

    IsVoid = BIT(0),
    IsArithmetic = BIT(1),
    IsPointer = BIT(2),
    IsEnum = BIT(3),
    IsArray = BIT(4),
    IsClass = BIT(5),
    IsDefaultConstructable = BIT(6),
  };
  HYP_CREATE_ENUM_FLAG_OPERATORS(MetaTypeTraits);

  enum class MetaPrimitiveType {
    None,

    Bool,

    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,

    Float32,
    Float64,

    Vector2,
    Vector3,
    Vector4,
    Quaternion,
    Matrix4x4,

    String,
    Color,

    EntityId
  };

  using MetaTrivialDestructor = void(*)(const void *);
  using MetaInPlaceConstructor = void *(*)(void *);

  namespace Internal {

    struct MetaAttributeNode {
      MetaAttributeNode *next;

      Any(*const key)();
      Any(*const value)();
      MetaAttribute(*const clazz)();
    };

    struct MetaBaseNode {
      MetaBaseNode **const underlying;
      MetaTypeNode *const parent;
      MetaBaseNode *next;

      MetaTypeNode *(*const ref)();
      void *(*const cast)(void *);
      MetaBase(*const clazz)();
    };

    struct MetaConversionNode {
      MetaConversionNode **const underlying;
      MetaTypeNode *const parent;
      MetaConversionNode *next;

      MetaTypeNode *(*const ref)();
      Any(*const convert)(const void *);
      MetaConversion(*const clazz)();
    };

    struct MetaConstructorNode {
      using size_type = std::size_t;

      MetaConstructorNode **const underlying;
      MetaTypeNode *const parent;
      MetaConstructorNode *next;
      MetaAttributeNode *attribute;
      const size_type size;

      MetaTypeNode *(*const arg)(size_type);
      Any(*const invoke)(Any *const);
      MetaConstructor(*const clazz)();
    };

    struct MetaDestructorNode {
      MetaDestructorNode **const underlying;
      MetaTypeNode *const parent;

      bool8(*const invoke)(MetaHandle);
      MetaDestructor(*const clazz)();
    };

    struct MetaPropertyNode {
      MetaPropertyNode **const underlying;
      std::size_t identifier;
      String name;
      MetaTypeNode *const parent;
      MetaPropertyNode *next;
      MetaAttributeNode *attribute;
      const bool8 is_const;
      const bool8 is_static;

      MetaTypeNode *(*const ref)();
      bool8(*const set)(MetaHandle, Any, Any);
      Any(*const get)(MetaHandle, Any);
      MetaProperty(*const clazz)();
    };

    struct MetaFunctionNode {
      using size_type = std::size_t;

      MetaFunctionNode **const underlying;
      std::size_t identifier;
      String name;
      MetaTypeNode *const parent;
      MetaFunctionNode *next;
      MetaAttributeNode *attribute;
      const size_type size;
      const bool8 is_const;
      const bool8 is_static;

      MetaTypeNode *(*const ret)();
      MetaTypeNode *(*const arg)(size_type);
      Any(*const invoke)(MetaHandle, Any *);
      MetaFunction(*const clazz)();
    };

    struct MetaTypeNode {
      using size_type = std::size_t;

      std::size_t identifier;
      String name;
      MetaTypeTraits traits;
      MetaPrimitiveType primitive_type;
      MetaTrivialDestructor trivial_destructor;
      MetaInPlaceConstructor in_place_constructor;
      size_type size;
      MetaTypeNode *next;
      MetaAttributeNode *attribute;

      bool8(*const compare)(const void *, const void *);
      MetaType(*const remove_pointer)();
      MetaType(*const clazz)();

      MetaBaseNode *base{ nullptr };
      MetaConversionNode *conv{ nullptr };
      MetaConstructorNode *ctor{ nullptr };
      MetaDestructorNode *dtor{ nullptr };
      MetaPropertyNode *data{ nullptr };
      MetaFunctionNode *func{ nullptr };
    };

    template<typename...>
    struct MetaInfoNode {
      inline static MetaTypeNode *type = nullptr;
    };

    template<typename Type>
    struct MetaInfoNode<Type> {
      inline static MetaTypeNode *type = nullptr;

      template<typename>
      inline static MetaBaseNode *base = nullptr;

      template<typename>
      inline static MetaConversionNode *conv = nullptr;

      template<typename>
      inline static MetaConstructorNode *ctor = nullptr;

      template<auto>
      inline static MetaDestructorNode *dtor = nullptr;

      template<auto...>
      inline static MetaPropertyNode *data = nullptr;

      template<auto>
      inline static MetaFunctionNode *func = nullptr;

      inline static MetaTypeNode *Resolve();
    };

    template<typename... Type>
    struct MetaTypeInfo : MetaInfoNode<std::remove_cv_t<std::remove_reference_t<Type>>...> { };

    template<typename Op, typename Node>
    void Iterate(Op op, const Node *curr) {
      while (curr) {
        op(curr);
        curr = curr->next;
      }
    }

    template<auto Member, typename Op>
    void Iterate(Op op, const MetaTypeNode *node) {
      if (node) {
        auto *curr = node->base;
        Iterate(op, node->*Member);

        while (curr) {
          Iterate<Member>(op, curr->ref());
          curr = curr->next;
        }
      }
    }

    template<typename Op, typename Node>
    auto FindIf(Op op, const Node *curr) {
      while (curr && !op(curr)) {
        curr = curr->next;
      }

      return curr;
    }

    template<auto Member, typename Op>
    auto FindIf(Op op, const MetaTypeNode *node) -> decltype(FindIf(op, node->*Member)) {
      decltype(FindIf(op, node->*Member)) ret = nullptr;

      if (node) {
        ret = FindIf(op, node->*Member);
        auto *curr = node->base;

        while (curr && !ret) {
          ret = FindIf<Member>(op, curr->ref());
          curr = curr->next;
        }
      }

      return ret;
    }

    template<typename Type>
    const Type *TryCast(const MetaTypeNode *node, void *instance) {
      const auto *type = MetaTypeInfo<Type>::Resolve();
      void *ret = nullptr;

      if (node == type) {
        ret = instance;
      } else {
        const auto *base = FindIf<&MetaTypeNode::base>([type](auto *candidate) {
          return candidate->ref() == type;
        }, node);

        ret = base ? base->cast(instance) : nullptr;
      }

      return static_cast<const Type *>(ret);
    }

    template<auto Member>
    inline bool8 CanCastOrConvert(const MetaTypeNode *from, const MetaTypeNode *to) {
      return (from == to) || FindIf<Member>([to](auto *node) {
        return node->ref() == to;
      }, from);
    }

    template<typename... Args, std::size_t... Indexes>
    inline auto Constructor(std::index_sequence<Indexes...>, const MetaTypeNode *node) {
      return Internal::FindIf([](auto *candidate) {
        return candidate->size == sizeof...(Args) &&
          (([](auto *from, auto *to) {
          return Internal::CanCastOrConvert<&Internal::MetaTypeNode::base>(from, to)
            || Internal::CanCastOrConvert<&Internal::MetaTypeNode::conv>(from, to);
        }(Internal::MetaTypeInfo<Args>::Resolve(), candidate->arg(Indexes))) && ...);
      }, node->ctor);
    }

  }

  class Any {
  private:
    using storage_type = std::aligned_storage_t<sizeof(void *), alignof(void *)>;
    using copy_fn_type = void *(storage_type &, const void *);
    using destroy_fn_type = void(void *);
    using steal_fn_type = void *(storage_type &, void *, destroy_fn_type *);
  public:
    Any() : storage{ }, instance{ nullptr }, node{ nullptr }, destroy_fn{ nullptr }, copy_fn{ nullptr }, steal_fn{ nullptr } { }

    template<typename Type, typename... Args>
    explicit Any(std::in_place_type_t<Type>, [[maybe_unused]] Args &&... args) : Any{ } {
      node = Internal::MetaTypeInfo<Type>::Resolve();

      if constexpr (!std::is_void_v<Type>) {
        using traits_type = type_traits<std::remove_cv_t<std::remove_reference_t<Type>>>;
        instance = traits_type::instance(storage, std::forward<Args>(args)...);
        destroy_fn = &traits_type::destroy;
        copy_fn = &traits_type::copy;
        steal_fn = &traits_type::steal;
      }
    }

    template<typename Type>
    explicit Any(std::reference_wrapper<Type> type) : Any{ } {
      node = Internal::MetaTypeInfo<Type>::Resolve();
      instance = &type.get();
    }

    inline Any(MetaHandle MetaHandle);

    template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Type>>, Any>>>
    Any(Type &&type) : Any{ std::in_place_type<std::remove_cv_t<std::remove_reference_t<Type>>>, std::forward<Type>(type) } { }

    Any(const Any &other) : Any{ } {
      node = other.node;
      instance = other.copy_fn ? other.copy_fn(storage, other.instance) : other.instance;
      destroy_fn = other.destroy_fn;
      copy_fn = other.copy_fn;
      steal_fn = other.steal_fn;
    }

    Any(Any &&other) : Any{ } { Swap(*this, other); }

    ~Any() {
      if (destroy_fn) {
        destroy_fn(instance);
      }
    }
  public:
    inline Hyperion::MetaType GetType() const;
    const void *GetData() const { return instance; }
    void *GetData() { return const_cast<void *>(std::as_const(*this).GetData()); }

    template<typename Type>
    const Type *TryCast() const {
      return Internal::TryCast<Type>(node, instance);
    }

    template<typename Type>
    Type *TryCast() {
      return const_cast<Type *>(std::as_const(*this).TryCast<Type>());
    }

    template<typename Type>
    const Type &Cast() const {
      auto *actual = TryCast<Type>();
      assert(actual);
      return *actual;
    }

    template<typename Type>
    Type &Cast() {
      return const_cast<Type &>(std::as_const(*this).Cast<Type>());
    }

    template<typename Type>
    Any Convert() const {
      Any Any{ };

      if (const auto *type = Internal::MetaTypeInfo<Type>::Resolve(); node == type) {
        Any = *static_cast<const Type *>(instance);
      } else {
        const auto *conv = Internal::FindIf<&Internal::MetaTypeNode::conv>([type](auto *other) {
          return other->ref() == type;
        }, node);

        if (conv) {
          Any = conv->convert(instance);
        }
      }

      return Any;
    }

    template<typename Type>
    bool8 Convert() {
      bool8 valid = (node == Internal::MetaTypeInfo<Type>::Resolve());

      if (!valid) {
        if (auto Any = std::as_const(*this).Convert<Type>(); Any) {
          Swap(Any, *this);
          valid = true;
        }
      }

      return valid;
    }

    template<typename Type, typename... Args>
    void Emplace(Args&& ... args) {
      *this = Any{ std::in_place_type_t<Type>{ }, std::forward<Args>(args)... };
    }

    template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Type>>, Any>>>
    Any &operator=(Type &&type) { return (*this = Any{ std::forward<Type>(type) }); }
    Any &operator=(const Any &other) { return (*this = Any{ other }); }
    Any &operator=(Any &&other) {
      Any Any{ std::move(other) };
      Swap(Any, *this);
      return *this;
    }

    explicit operator bool8() const { return node; }
    bool8 operator==(const Any &other) const { return node == other.node && (!node || node->compare(instance, other.instance)); }

    friend void Swap(Any &lhs, Any &rhs) {
      if (lhs.steal_fn && rhs.steal_fn) {
        storage_type buffer;
        auto *temp = lhs.steal_fn(buffer, lhs.instance, lhs.destroy_fn);
        lhs.instance = rhs.steal_fn(lhs.storage, rhs.instance, rhs.destroy_fn);
        rhs.instance = lhs.steal_fn(rhs.storage, temp, lhs.destroy_fn);
      } else if (lhs.steal_fn) {
        lhs.instance = lhs.steal_fn(rhs.storage, lhs.instance, lhs.destroy_fn);
        std::swap(rhs.instance, lhs.instance);
      } else if (rhs.steal_fn) {
        rhs.instance = rhs.steal_fn(lhs.storage, rhs.instance, rhs.destroy_fn);
        std::swap(rhs.instance, lhs.instance);
      } else {
        std::swap(lhs.instance, rhs.instance);
      }

      std::swap(lhs.node, rhs.node);
      std::swap(lhs.destroy_fn, rhs.destroy_fn);
      std::swap(lhs.copy_fn, rhs.copy_fn);
      std::swap(lhs.steal_fn, rhs.steal_fn);
    }
  private:
    template<typename Type, typename = std::void_t<>>
    struct type_traits {
      template<typename... Args>
      static void *instance(storage_type &storage, Args &&... args) {
        auto instance = std::make_unique<Type>(std::forward<Args>(args)...);
        new (&storage) Type *{ instance.get() };
        return instance.release();
      }

      static void destroy(void *instance) {
        auto *node = Internal::MetaTypeInfo<Type>::Resolve();
        auto *actual = static_cast<Type *>(instance);
        [[maybe_unused]] const bool8 destroyed = node->clazz().Destroy(*actual);
        assert(destroyed);
        delete actual;
      }

      static void *copy(storage_type &storage, const void *other) {
        auto instance = std::make_unique<Type>(*static_cast<const Type *>(other));
        new (&storage) Type *{ instance.get() };
        return instance.release();
      }

      static void *steal(storage_type &to, void *from, destroy_fn_type *) {
        auto *instance = static_cast<Type *>(from);
        new (&to) Type *{ instance };
        return instance;
      }
    };

    template<typename Type>
    struct type_traits<Type, std::enable_if_t<sizeof(Type) <= sizeof(void *) && std::is_nothrow_move_constructible_v<Type>>> {
      template<typename... Args>
      static void *instance(storage_type &storage, Args &&... args) {
        return new (&storage) Type{ std::forward<Args>(args)... };
      }

      static void destroy(void *instance) {
        auto *node = Internal::MetaTypeInfo<Type>::Resolve();
        auto *actual = static_cast<Type *>(instance);
        [[maybe_unused]] const bool8 destroyed = node->clazz().Destroy(*actual);
        assert(destroyed);
        actual->~Type();
      }

      static void *copy(storage_type &storage, const void *instance) {
        return new (&storage) Type{ *static_cast<const Type *>(instance) };
      }

      static void *steal(storage_type &to, void *from, destroy_fn_type *destroy_fn) {
        void *instance = new (&to) Type{ std::move(*static_cast<Type *>(from)) };
        destroy_fn(from);
        return instance;
      }
    };
  private:
    storage_type storage;
    void *instance;
    const Internal::MetaTypeNode *node;
    destroy_fn_type *destroy_fn;
    copy_fn_type *copy_fn;
    steal_fn_type *steal_fn;
  private:
    friend class MetaHandle;
  };

  inline bool8 operator!=(const Any &lhs, const Any &rhs) { return !(lhs == rhs); }

  class MetaHandle {
  public:
    inline MetaHandle() : node{ nullptr }, instance{ nullptr } { }
    inline MetaHandle(MetaType type, void *instance);
    inline MetaHandle(Any &Any) : node{ Any.node }, instance{ Any.instance } { }
    template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Type>>, MetaHandle>>>
    inline MetaHandle(Type &obj) : node{ Internal::MetaTypeInfo<Type>::Resolve() }, instance{ &obj } { }
  public:
    inline Hyperion::MetaType GetType() const;
    inline const void *GetData() const { return instance; }
    inline void *GetData() { return const_cast<void *>(std::as_const(*this).GetData()); }

    inline explicit operator bool8() const { return instance; }
  private:
    const Internal::MetaTypeNode *node;
    void *instance;
  private:
    friend class Any;
  };

  class MetaAttribute {
  public:
    MetaAttribute() : node{ nullptr } { }
  private:
    MetaAttribute(const Internal::MetaAttributeNode *curr) : node{ curr } { }
  public:
    inline Any GetKey() const { return node->key(); }
    inline Any GetValue() const { return node->value(); }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaAttribute &other) const { return node == other.node; }
  private:
    const Internal::MetaAttributeNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaAttribute &lhs, const MetaAttribute &rhs) { return !(lhs == rhs); }

  class MetaBase {
  public:
    MetaBase() : node{ nullptr } { }
  private:
    MetaBase(const Internal::MetaBaseNode *curr) : node{ curr } { }
  public:
    inline Hyperion::MetaType GetParentType() const;
    inline Hyperion::MetaType GetType() const;

    void *Cast(void *instance) const { return node->cast(instance); }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaBase &other) const { return node == other.node; }
  private:
    const Internal::MetaBaseNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaBase &lhs, const MetaBase &rhs) { return !(lhs == rhs); }

  class MetaConversion {
  public:
    MetaConversion() : node{ nullptr } { }
  private:
    MetaConversion(const Internal::MetaConversionNode *curr) : node{ curr } { }
  public:
    inline Hyperion::MetaType GetParentType() const;
    inline Hyperion::MetaType GetType() const;

    Any Convert(const void *instance) const { return node->convert(instance); }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaConversion &other) const { return node == other.node; }
  private:
    const Internal::MetaConversionNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaConversion &lhs, const MetaConversion &rhs) { return !(lhs == rhs); }

  class MetaConstructor {
  public:
    using size_type = typename Internal::MetaConstructorNode::size_type;
  public:
    MetaConstructor() : node{ nullptr } { }
  private:
    MetaConstructor(const Internal::MetaConstructorNode *curr) : node{ curr } { }
  public:
    inline Hyperion::MetaType GetParentType() const;
    size_type GetSize() const { return node->size; }
    Hyperion::MetaType GetArgumentType(size_type index) const;

    template<typename... Args>
    Any Invoke(Args &&... args) const {
      std::array<Any, sizeof...(Args)> arguments{ {std::forward<Args>(args)...} };
      Any Any{ };

      if (sizeof...(Args) == GetSize()) {
        Any = node->invoke(arguments.MetaProperty());
      }

      return Any;
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaAttribute>, void>
      ForEachAttribute(Op op) const {
      Internal::Iterate([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node->attribute);
    }

    template<typename Key>
    std::enable_if_t<!std::is_invocable_v<Key, Hyperion::MetaAttribute>, Hyperion::MetaAttribute>
      GetAttribute(Key &&key) const {
      const auto *curr = Internal::FindIf([key = Any{ std::forward<Key>(key) }](auto *candidate) {
        return candidate->key() == key;
      }, node->attribute);

      return curr ? curr->clazz() : Hyperion::MetaAttribute{ };
    }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaConstructor &other) const { return node == other.node; }
  private:
    const Internal::MetaConstructorNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaConstructor &lhs, const MetaConstructor &rhs) { return !(lhs == rhs); }

  class MetaDestructor {
  public:
    MetaDestructor() : node{ nullptr } { }
  private:
    MetaDestructor(const Internal::MetaDestructorNode *curr) : node{ curr } { }
  public:
    inline Hyperion::MetaType GetParentType() const;

    bool8 Invoke(MetaHandle MetaHandle) const { return node->invoke(MetaHandle); }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaDestructor &other) const { return node == other.node; }
  private:
    const Internal::MetaDestructorNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaDestructor &lhs, const MetaDestructor &rhs) { return !(lhs == rhs); }

  class MetaProperty {
  public:
    MetaProperty() : node{ nullptr } { }
  private:
    MetaProperty(const Internal::MetaPropertyNode *curr) : node{ curr } { }
  public:
    String GetName() const { return node->name; }
    inline Hyperion::MetaType GetParentType() const;
    inline Hyperion::MetaType GetType() const;
    bool8 IsConst() const { return node->is_const; }
    bool8 IsStatic() const { return node->is_static; }

    template<typename Type>
    bool8 Set(MetaHandle MetaHandle, Type &&value) const {
      return node->set(MetaHandle, Any{ }, std::forward<Type>(value));
    }

    template<typename Type>
    bool8 Set(MetaHandle MetaHandle, std::size_t index, Type &&value) const {
      return node->set(MetaHandle, index, std::forward<Type>(value));
    }

    Any Get(MetaHandle MetaHandle) const {
      return node->get(MetaHandle, Any{ });
    }

    Any Get(MetaHandle MetaHandle, std::size_t index) const {
      return node->get(MetaHandle, index);
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaAttribute>, void>
      ForEachAttribute(Op op) const {
      Internal::Iterate([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node->attribute);
    }

    template<typename Key>
    std::enable_if_t<!std::is_invocable_v<Key, Hyperion::MetaAttribute>, Hyperion::MetaAttribute>
      GetAttribute(Key &&key) const {
      const auto *curr = Internal::FindIf([key = Any{ std::forward<Key>(key) }](auto *candidate) {
        return candidate->key() == key;
      }, node->attribute);

      return curr ? curr->clazz() : Hyperion::MetaAttribute{ };
    }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaProperty &other) const { return node == other.node; }
  private:
    const Internal::MetaPropertyNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaProperty &lhs, const MetaProperty &rhs) { return !(lhs == rhs); }

  class MetaFunction {
  public:
    using size_type = typename Internal::MetaFunctionNode::size_type;
  public:
    MetaFunction() : node{ nullptr } { }
  private:
    MetaFunction(const Internal::MetaFunctionNode *curr) : node{ curr } { }
  public:
    String GetName() const { return node->name; }
    inline Hyperion::MetaType GetParentType() const;
    inline Hyperion::MetaType GetReturnType() const;
    inline Hyperion::MetaType GetArgumentType(size_type index) const;
    size_type GetSize() const { return node->size; }
    bool8 IsConst() const { return node->is_const; }
    bool8 IsStatic() const { return node->is_static; }

    template<typename... Args>
    Any Invoke(MetaHandle MetaHandle, Args &&... args) const {
      // makes aliasing on the values and passes forward references if any
      std::array<Any, sizeof...(Args)> arguments{ {Hyperion::MetaHandle{args}...} };
      Any Any{ };

      if (sizeof...(Args) == GetSize()) {
        Any = node->invoke(MetaHandle, arguments.data());
      }

      return Any;
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaAttribute>, void>
      ForEachAttribute(Op op) const {
      Internal::Iterate([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node->attribute);
    }

    template<typename Key>
    std::enable_if_t<!std::is_invocable_v<Key, Hyperion::MetaAttribute>, Hyperion::MetaAttribute>
      GetAttribute(Key &&key) const {
      const auto *curr = Internal::FindIf([key = Any{ std::forward<Key>(key) }](auto *candidate) {
        return candidate->key() == key;
      }, node->attribute);

      return curr ? curr->clazz() : Hyperion::MetaAttribute{ };
    }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaFunction &other) const { return node == other.node; }
  private:
    const Internal::MetaFunctionNode *node;
  private:
    template<typename> friend class MetaFactory;
  };

  inline bool8 operator!=(const MetaFunction &lhs, const MetaFunction &rhs) { return !(lhs == rhs); }

  class MetaType {
  public:
    using size_type = typename Internal::MetaTypeNode::size_type;
  public:
    MetaType() : node{ nullptr } { }
  private:
    MetaType(const Internal::MetaTypeNode *curr) : node{ curr } { }
  public:
    String GetName() const { return node->name; }
    MetaPrimitiveType GetPrimitiveType() const { return node->primitive_type; }
    MetaTrivialDestructor GetTrivialDestructor() const { return node->trivial_destructor; }
    MetaInPlaceConstructor GetInPlaceConstructor() const { return node->in_place_constructor; }
    size_type GetSize() const { return node->size; }
    bool8 IsVoid() const { return (node->traits & MetaTypeTraits::IsVoid) == MetaTypeTraits::IsVoid; }
    bool8 IsArithmetic() const { return (node->traits & MetaTypeTraits::IsArithmetic) == MetaTypeTraits::IsArithmetic; }
    bool8 IsPointer() const { return (node->traits & MetaTypeTraits::IsPointer) == MetaTypeTraits::IsPointer; }
    bool8 IsEnum() const { return (node->traits & MetaTypeTraits::IsEnum) == MetaTypeTraits::IsEnum; }
    bool8 IsArray() const { return (node->traits & MetaTypeTraits::IsArray) == MetaTypeTraits::IsArray; }
    bool8 IsClass() const { return (node->traits & MetaTypeTraits::IsClass) == MetaTypeTraits::IsClass; }
    bool8 IsDefaultConstructable() const { return (node->traits & MetaTypeTraits::IsDefaultConstructable) == MetaTypeTraits::IsDefaultConstructable; }
    Hyperion::MetaType RemovePointer() const { return node->remove_pointer(); }

    template<typename T>
    bool8 IsDerivedFrom() {
      Internal::MetaTypeNode *type_node = Internal::MetaInfoNode<T>::Resolve();
      return IsDerivedFrom(type_node->clazz());
    }

    bool8 IsDerivedFrom(MetaType type) {
      if (*this == type) {
        return true;
      }

      bool8 has_base = false;
      ForEachBase([type, &has_base](MetaBase base) {
        if (base.GetType() == type) {
          has_base = true;
        }
      });

      return has_base;
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaBase>, void>
      ForEachBase(Op op) const {
      Internal::Iterate<&Internal::MetaTypeNode::base>([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node);
    }

    Hyperion::MetaBase GetBase(const String &name) const {
      const std::size_t identifier = std::hash<String>{ }(name);
      const auto *curr = Internal::FindIf<&Internal::MetaTypeNode::base>([identifier](auto *candidate) {
        return candidate->ref()->identifier == identifier;
      }, node);

      return curr ? curr->clazz() : Hyperion::MetaBase{ };
    }

    template<typename Op>
    void ForEachConversion(Op op) const {
      Internal::Iterate<&Internal::MetaTypeNode::conv>([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node);
    }

    template<typename Type>
    Hyperion::MetaConversion GetConversion() const {
      const auto *curr = Internal::FindIf<&Internal::MetaTypeNode::conv>([type = Internal::MetaTypeInfo<Type>::Resolve()](auto *candidate) {
        return candidate->ref() == type;
      }, node);

      return curr ? curr->clazz() : Hyperion::MetaConversion{ };
    }

    template<typename Op>
    void ForEachConstructor(Op op) const {
      Internal::Iterate([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node->ctor);
    }

    template<typename... Args>
    Hyperion::MetaConstructor GetConstructor() const {
      const auto *curr = Internal::Constructor<Args...>(std::make_index_sequence<sizeof...(Args)>{ }, node);
      return curr ? curr->clazz() : Hyperion::MetaConstructor{ };
    }

    Hyperion::MetaDestructor GetDestructor() const {
      return node->dtor ? node->dtor->clazz() : Hyperion::MetaDestructor{ };
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaProperty>, void>
      ForEachProperty(Op op) const {
      Internal::Iterate<&Internal::MetaTypeNode::data>([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node);
    }

    Hyperion::MetaProperty GetProperty(const String &name) const {
      const std::size_t identifier = std::hash<String>{ }(name);
      const auto *curr = Internal::FindIf<&Internal::MetaTypeNode::data>([identifier](auto *candidate) {
        return candidate->identifier == identifier;
      }, node);

      return curr ? curr->clazz() : Hyperion::MetaProperty{ };
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaFunction>, void>
      ForEachFunction(Op op) const {
      Internal::Iterate<&Internal::MetaTypeNode::func>([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node);
    }

    Hyperion::MetaFunction GetFunction(const String &name) const {
      const std::size_t identifier = std::hash<String>{ }(name);
      const auto *curr = Internal::FindIf<&Internal::MetaTypeNode::func>([identifier](auto *candidate) {
        return candidate->identifier == identifier;
      }, node);

      return curr ? curr->clazz() : Hyperion::MetaFunction{ };
    }

    template<typename... Args>
    Any Construct(Args &&... args) const {
      std::array<Any, sizeof...(Args)> arguments{ {std::forward<Args>(args)...} };
      Any Any{ };

      Internal::FindIf<&Internal::MetaTypeNode::ctor>([data = arguments.data(), &Any](auto *curr)->bool8 {
        if (curr->size == sizeof...(args)) {
          Any = curr->invoke(data);
        }

        return static_cast<bool8>(Any);
      }, node);

      return Any;
    }

    bool8 Destroy(MetaHandle MetaHandle) const {
      return (MetaHandle.GetType() == node->clazz()) && (!node->dtor || node->dtor->invoke(MetaHandle));
    }

    template<typename Op>
    std::enable_if_t<std::is_invocable_v<Op, Hyperion::MetaAttribute>, void>
      ForEachAttribute(Op op) const {
      Internal::Iterate<&Internal::MetaTypeNode::attribute>([op = std::move(op)](auto *curr) {
        op(curr->clazz());
      }, node);
    }

    template<typename Key>
    std::enable_if_t<!std::is_invocable_v<Key, Hyperion::MetaAttribute>, Hyperion::MetaAttribute>
      GetAttribute(Key &&key) const {
      const auto *curr = Internal::FindIf<&Internal::MetaTypeNode::attribute>([key = Any{ std::forward<Key>(key) }](auto *candidate) {
        return candidate->key() == key;
      }, node);

      return curr ? curr->clazz() : Hyperion::MetaAttribute{ };
    }

    explicit operator bool8() const { return node; }
    bool8 operator==(const MetaType &other) const { return node == other.node; }
  private:
    const Internal::MetaTypeNode *node;
  private:
    friend class MetaHandle;
    template<typename...>
    friend struct Internal::MetaInfoNode;
    friend struct std::hash<MetaType>;
  };

  inline bool8 operator!=(const MetaType &lhs, const MetaType &rhs) { return !(lhs == rhs); }

  inline Any::Any(MetaHandle MetaHandle) : Any{ } {
    node = MetaHandle.node;
    instance = MetaHandle.instance;
  }

  inline MetaHandle::MetaHandle(MetaType type, void *instance) : node(type.node), instance(instance) { }

  inline Hyperion::MetaType Any::GetType() const { return node ? node->clazz() : Hyperion::MetaType{ }; }
  inline Hyperion::MetaType MetaHandle::GetType() const { return node ? node->clazz() : Hyperion::MetaType{ }; }
  inline Hyperion::MetaType MetaBase::GetParentType() const { return node->parent->clazz(); }
  inline Hyperion::MetaType MetaBase::GetType() const { return node->ref()->clazz(); }
  inline Hyperion::MetaType MetaConversion::GetParentType() const { return node->parent->clazz(); }
  inline Hyperion::MetaType MetaConversion::GetType() const { return node->ref()->clazz(); }
  inline Hyperion::MetaType MetaConstructor::GetParentType() const { return node->parent->clazz(); }
  inline Hyperion::MetaType MetaConstructor::GetArgumentType(size_type index) const { return index < GetSize() ? node->arg(index)->clazz() : Hyperion::MetaType{ }; }
  inline Hyperion::MetaType MetaDestructor::GetParentType() const { return node->parent->clazz(); }
  inline Hyperion::MetaType MetaProperty::GetParentType() const { return node->parent->clazz(); }
  inline Hyperion::MetaType MetaProperty::GetType() const { return node->ref()->clazz(); }
  inline Hyperion::MetaType MetaFunction::GetParentType() const { return node->parent->clazz(); }
  inline Hyperion::MetaType MetaFunction::GetReturnType() const { return node->ret()->clazz(); }
  inline Hyperion::MetaType MetaFunction::GetArgumentType(size_type index) const { return index < GetSize() ? node->arg(index)->clazz() : Hyperion::MetaType{ }; }

  namespace Internal {

    template<typename Type, typename = std::enable_if_t<!std::is_void_v<Type> && !std::is_function_v<Type>>>
    static auto Compare(int, const void *lhs, const void *rhs) -> decltype(std::declval<Type>() == std::declval<Type>(), bool8{ }) {
      return *static_cast<const Type *>(lhs) == *static_cast<const Type *>(rhs);
    }

    template<typename>
    static bool8 Compare(char, const void *lhs, const void *rhs) { return lhs == rhs; }

    template<typename Type>
    inline MetaTypeNode *MetaInfoNode<Type>::Resolve() {
      if (!type) {
        static MetaTypeNode node{
            { },
            { },
            MetaTypeTraits::None |
            (std::is_void_v<Type> ? MetaTypeTraits::IsVoid : MetaTypeTraits::None) |
            (std::is_arithmetic_v<Type> ? MetaTypeTraits::IsArithmetic : MetaTypeTraits::None) |
            (std::is_pointer_v<Type> ? MetaTypeTraits::IsPointer : MetaTypeTraits::None) |
            (std::is_enum_v<Type> ? MetaTypeTraits::IsEnum : MetaTypeTraits::None) |
            (std::is_array_v<Type> ? MetaTypeTraits::IsArray : MetaTypeTraits::None) |
            (std::is_class_v<Type> ? MetaTypeTraits::IsClass : MetaTypeTraits::None) |
            (std::is_default_constructible_v<Type> ? MetaTypeTraits::IsDefaultConstructable : MetaTypeTraits::None),
            MetaPrimitiveType::None,
            nullptr,
            nullptr,
            sizeof(Type),
            nullptr,
            nullptr,
            [](const void *lhs, const void *rhs) {
                return Compare<Type>(0, lhs, rhs);
            },
            []() -> Hyperion::MetaType {
                return Internal::MetaTypeInfo<std::remove_pointer_t<Type>>::Resolve();
            },
            []() -> Hyperion::MetaType {
                return &node;
            }
        };

        type = &node;
      }

      return type;
    }

  }

}

namespace std {

  template <>
  struct hash<Hyperion::MetaType> {
    std::size_t operator()(const Hyperion::MetaType &type) const {
      return hash<const Hyperion::Internal::MetaTypeNode *>()(type.node);
    }
  };

}
