#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <utility>

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/log.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    template<typename T>
    struct Ok {
        Ok() = default;
        ~Ok() = default;

        Ok(const T &value) : value(value) { }
        Ok(T &&value) : value(std::move(value)) { }

        T value;
    };

    template<typename E>
    struct Err {
        Err() = default;
        ~Err() = default;

        Err(const E &error) : error(error) { }
        Err(E &&error) : error(std::move(error)) { }

        E error;
    };

    template<typename T, typename E>
    class Result {
    public:
        Result() = default;
        ~Result() = default;

        Result(Ok<T> value) : m_okay_value(std::move(value)), m_error_value{ }, m_okay_contains_value(true) { }
        Result(Err<E> error) : m_okay_value{ }, m_error_value(std::move(error)), m_okay_contains_value(false) { }

        bool &IsOk() { return m_okay_contains_value; }
        const bool &IsOk() const { return m_okay_contains_value; }
        bool IsErr() { return !m_okay_contains_value; }
        const bool IsErr() const { return !m_okay_contains_value; }

        T &Expect(const char *msg) {
            HYP_ASSERT_MESSAGE(m_okay_contains_value, msg);

            return m_okay_value.value;
        }

        const T &Expect(const char *msg) const {
            HYP_ASSERT_MESSAGE(m_okay_contains_value, msg);

            return m_okay_value.value;
        }

        T &Expect(const String &msg) {
            HYP_ASSERT_MESSAGE(m_okay_contains_value, msg);
        
            return m_okay_value.value;
        }
        
        const T &Expect(const String &msg) const {
            HYP_ASSERT_MESSAGE(m_okay_contains_value, msg);
            
            return m_okay_value.value;
        }

        T &Unwrap() {
            HYP_ASSERT(m_okay_contains_value);

            return m_okay_value.value;
        }

        const T &Unwrap() const {
            HYP_ASSERT(m_okay_contains_value);

            return m_okay_value.value;
        }

        T &UnwrapOr(T &optional_value) {
            if (m_okay_contains_value) {
                return m_okay_value.value;
            } else {
                return optional_value;
            }
        }

        const T &UnwrapOr(const T &optional_value) const {
            if (m_okay_contains_value) {
                return m_okay_value.value;
            } else {
                return optional_value;
            }
        }

        E &UnwrapError() {
            HYP_ASSERT(!m_okay_contains_value);

            return m_error_value.error;
        }

        const E &UnwrapError() const {
            HYP_ASSERT(!m_okay_contains_value);

            return m_error_value.error;
        }

        bool operator==(const Ok<T> &value) {
            return m_okay_value.value == value.value;
        }

        bool operator==(const Ok<T> &value) const {
            return m_okay_value.value == value.value;
        }

        bool operator==(const Err<E> &error) {
            return m_error_value.error == error.error;
        }

        bool operator==(const Err<E> &error) const {
            return m_error_value.error == error.error;
        }

        bool operator!=(const Ok<T> &value) {
            return m_okay_value.value != value.value;
        }

        bool operator!=(const Ok<T> &value) const {
            return m_okay_value.value != value.value;
        }

        bool operator!=(const Err<E> &error) {
            return m_error_value.error != error.error;
        }

        bool operator!=(const Err<E> &error) const {
            return m_error_value.error != error.error;
        }
    private:
        Ok<T> m_okay_value;
        Err<E> m_error_value;

        bool m_okay_contains_value;
    };

}