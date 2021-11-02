//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <unordered_set>

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//--------------------------------------------------------------
namespace Hyperion {

    template<typename T>
    class Set {
    public:
        using Iterator = typename std::unordered_set<T>::iterator;
        using ConstIterator = typename std::unordered_set<T>::const_iterator;
    public:
        inline Set() { }
        template<typename Iterator>
        inline Set(Iterator first, Iterator last) : m_set(first, last) { }
        inline Set(std::initializer_list<T> init) : m_set(init) { }
    public:
        inline uint64 GetLength() const { return m_set.size(); }
        inline bool8 IsEmpty() const { return m_set.empty(); }

        inline bool8 Contains(const T &item) const { return std::find(begin(), end(), item) != end(); }

        inline auto Insert(const T &item) { return m_set.insert(item); }
        inline auto Insert(T &&item) { return m_set.insert(item); }

        inline void Remove(const T &item) { m_set.erase(item); }

        inline void Clear() { m_set.clear(); }
        inline void Reserve(uint64 capacity) { m_set.reserve(capacity); }

        inline Iterator begin() { return m_set.begin(); }
        inline ConstIterator begin() const { return m_set.begin(); }
        inline ConstIterator cbegin() const { return m_set.cbegin(); }
        inline Iterator end() { return m_set.end(); }
        inline ConstIterator end() const { return m_set.end(); }
        inline ConstIterator cend() const { return m_set.cend(); }
    private:
        std::unordered_set<T> m_set;
    };

}