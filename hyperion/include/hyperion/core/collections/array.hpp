//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <vector>

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//--------------------------------------------------------------
namespace Hyperion {

  template<typename T>
  class Array {
  public:
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;
    using ReverseIterator = typename std::vector<T>::reverse_iterator;
    using ConstReverseIterator = typename std::vector<T>::const_reverse_iterator;
  public:
    Array() = default;
    inline Array(uint64 size) : m_vector(size) { }

    template<typename Iterator>
    inline Array(Iterator first, Iterator last) : m_vector(first, last) { }

    inline Array(std::initializer_list<T> init) : m_vector(init) { }
  public:
    inline uint64 GetLength() const { return m_vector.size(); }
    inline uint64 GetCapacity() const { return m_vector.capacity(); }
    inline bool8 IsEmpty() const { return m_vector.empty(); }

    inline T *GetData() { return m_vector.data(); }
    inline const T *GetData() const { return m_vector.data(); }

    inline T &Get(uint64 index) { return m_vector.at(index); }
    inline const T &Get(uint64 index) const { return m_vector.at(index); }
    inline T &GetFirst() { return m_vector.front(); }
    inline const T &GetFirst() const { return m_vector.front(); }
    inline T &GetLast() { return m_vector.back(); }
    inline const T &GetLast() const { return m_vector.back(); }

    inline bool8 Contains(const T &item) const { return std::find(begin(), end(), item) != end(); }

    inline void Add(const T &item) { m_vector.push_back(item); }
    inline void Add(T &&item) { m_vector.push_back(item); }

    inline bool8 Remove(const T &item) {
      auto it = std::remove(begin(), end(), item);
      if (it == end()) {
        return false;
      } else {
        m_vector.erase(it);
        return true;
      }
    }

    inline void RemoveLast() { m_vector.pop_back(); }

    inline void Clear() { m_vector.clear(); }
    inline void Reserve(uint64 capacity) { m_vector.reserve(capacity); }
    inline void Resize(uint64 size) { m_vector.resize(size); }
    inline void Resize(uint64 size, const T &value) { m_vector.resize(size, value); }
    inline void ShrinkToFit() { m_vector.shrink_to_fit(); }

    inline Iterator begin() { return m_vector.begin(); }
    inline ConstIterator begin() const { return m_vector.begin(); }
    inline ConstIterator cbegin() const { return m_vector.cbegin(); }
    inline Iterator end() { return m_vector.end(); }
    inline ConstIterator end() const { return m_vector.end(); }
    inline ConstIterator cend() const { return m_vector.cend(); }

    inline ReverseIterator rbegin() { return m_vector.rbegin(); }
    inline ConstReverseIterator rbegin() const { return m_vector.rbegin(); }
    inline ConstReverseIterator crbegin() const { return m_vector.crbegin(); }
    inline ReverseIterator rend() { return m_vector.rend(); }
    inline ConstReverseIterator rend() const { return m_vector.rend(); }
    inline ConstReverseIterator crend() const { return m_vector.crend(); }

    inline T &operator[](uint64 index) { return m_vector[index]; }
    inline const T &operator[](uint64 index) const { return m_vector[index]; }
  private:
    std::vector<T> m_vector;
  };

}
