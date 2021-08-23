#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <vector>

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//--------------------------------------------------------------
namespace Hyperion {

    template<typename T>
    class List {
        using ListIterator = typename std::vector<T>::iterator;
        using ListConstIterator = typename std::vector<T>::const_iterator;
        using ListReverseIterator = typename std::vector<T>::reverse_iterator;
        using ListConstReverseIterator = typename std::vector<T>::const_reverse_iterator;
    public:
        inline List() { }
        inline List(uint64 size) : m_vector(size) { }
        template<typename Iterator>
        inline List(Iterator first, Iterator last) : m_vector(first, last) { }
        inline List(std::initializer_list<T> init) : m_vector(init) { }
    public:
        inline uint64 GetLength() const { return m_vector.size(); }
        inline uint64 GetCapacity() const { return m_vector.capacity(); }
        inline bool IsEmpty() const { return m_vector.empty(); }

        inline T *GetData() { return m_vector.data(); }
        inline const T *GetData() const { return m_vector.data(); }

        inline T &Get(uint64 index) { m_vector.at(index); }
        inline const T &Get(uint64 index) const { m_vector.at(index); }
        inline T &GetFirst() { m_vector.front(); }
        inline const T &GetFirst() const { m_vector.front(); }
        inline T &GetLast() { m_vector.back(); }
        inline const T &GetLast() const { m_vector.back(); }

        inline bool Contains(const T &item) const { return std::find(begin(), end(), item) != end(); }

        inline void Add(const T &item) { m_vector.push_back(item); }
        inline void Add(T &&item) { m_vector.push_back(item); }
        
        inline void Remove(const T &item) { m_vector.erase(std::remove(begin(), end(), item)); }
        inline void Remove(ListIterator first, ListIterator last, const T &item) { m_vector.erase(std::remove(first, last, item)); }
        inline void RemoveLast() { m_vector.pop_back(); }

        inline void Clear() { m_vector.clear(); }
        inline void Reserve(uint64 capacity) { m_vector.reserve(capacity); }
        inline void Resize(uint64 size) { m_vector.resize(size); }
        inline void ShrinkToFit() { m_vector.shrink_to_fit(); }

        inline ListIterator begin() { return m_vector.begin(); }
        inline ListConstIterator begin() const { return m_vector.begin(); }
        inline ListConstIterator cbegin() const { return m_vector.cbegin(); }
        inline ListIterator end() { return m_vector.end(); }
        inline ListConstIterator end() const { return m_vector.end(); }
        inline ListConstIterator cend() const { return m_vector.cend(); }

        inline ListReverseIterator rbegin() { return m_vector.rbegin(); }
        inline ListConstReverseIterator rbegin() const { return m_vector.rbegin(); }
        inline ListConstReverseIterator crbegin() const { return m_vector.crbegin(); }
        inline ListReverseIterator rend() { return m_vector.rend(); }
        inline ListConstReverseIterator rend() const { return m_vector.rend(); }
        inline ListConstReverseIterator crend() const { return m_vector.crend(); }

        inline T &operator[](uint64 index) { return m_vector[index]; }
        inline const T &operator[](uint64 index) const { return m_vector[index]; }
    private:
        std::vector<T> m_vector;
    };

}