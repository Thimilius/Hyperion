#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <functional>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    template<auto>
    struct DelegateConnectionArguments {};

    template<auto Func>
    inline constexpr DelegateConnectionArguments<Func> ConnectionArguments { };

    template<typename>
    class Delegate;

    template<typename Ret, typename... Args>
    class Delegate<Ret(Args...)> {
    private:
        using FunctionType = Ret(const void *, Args...);
    public:
        Delegate() : m_function(nullptr), m_instance(nullptr) { }

        template<auto Candidate>
        Delegate(DelegateConnectionArguments<Candidate>) { Connect<Candidate>(); }

        template<auto Candidate, typename Type>
        Delegate(DelegateConnectionArguments<Candidate>, Type &&value_or_instance) { Connect<Candidate>(std::forward<Type>(value_or_instance)); }
    public:
        inline const void *GetInstance() { return m_instance; }

        template<auto Candidate>
        void Connect() {
            m_instance = nullptr;

            m_function = [](const void *, Args... args) -> Ret {
                return Ret(std::invoke(Candidate, std::forward<Args>(args)...));
            };
        }

        template<auto Candidate, typename Type>
        void Connect(Type &value_or_instance) {
            m_instance = &value_or_instance;

            m_function = [](const void *payload, Args... args) -> Ret {
                Type *curr = static_cast<Type *>(const_cast<void *>(payload));
                return Ret(std::invoke(Candidate, *curr, std::forward<Args>(args)...));
            };
        }

        template<auto Candidate, typename Type>
        void Connect(Type *value_or_instance) {
            m_instance = value_or_instance;

            m_function = [](const void *payload, Args... args) -> Ret {
                Type *curr = static_cast<Type *>(const_cast<void *>(payload));
                return Ret(std::invoke(Candidate, *curr, std::forward<Args>(args)...));
            };
        }

        void Reset() {
            m_function = nullptr;
            m_instance = nullptr;
        }

        Ret operator()(Args... args) const {
            return m_function(m_instance, std::forward<Args>(args)...);
        }

        explicit operator bool() const { return !(m_function == nullptr); }

        bool operator==(const Delegate<Ret(Args...)> &other) const { return m_function == other.m_function && m_instance == other.m_instance; }
        bool operator!=(const Delegate<Ret(Args...)> &other) const { return !(*this == other); }
    private:
        FunctionType *m_function;
        const void *m_instance;
    };

}