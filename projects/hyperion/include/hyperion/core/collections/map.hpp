//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <unordered_map>

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//--------------------------------------------------------------
namespace Hyperion {

  template<typename K, typename V>
  class Map {
  public:
    using Pair = typename std::unordered_map<K, V>::value_type;
    using Iterator = typename std::unordered_map<K, V>::iterator;
    using ConstIterator = typename std::unordered_map<K, V>::const_iterator;
  public:
    inline Map() { }
    inline Map(std::initializer_list<Pair> init) : m_map(init) { }
  public:
    inline uint64 GetLength() const { return m_map.size(); }
    inline bool8 IsEmpty() const { return m_map.empty(); }

    inline V &Get(const K &key) { return m_map.at(key); }
    inline const V &Get(const K &key) const { return m_map.at(key); }

    inline bool8 Contains(const K &key) const { return m_map.find(key) != m_map.end(); }
    inline Iterator Find(const K &key) { return m_map.find(key); }
    inline ConstIterator Find(const K &key) const { return m_map.find(key); }

    inline auto Insert(const K &key, const V &value) { return m_map.insert(std::make_pair(key, value)); }

    inline void Remove(const K &key) { m_map.erase(key); }

    inline void Clear() { m_map.clear(); }
    inline void Reserve(uint64 capacity) { m_map.reserve(capacity); }

    inline Iterator begin() { return m_map.begin(); }
    inline ConstIterator begin() const { return m_map.begin(); }
    inline ConstIterator cbegin() const { return m_map.cbegin(); }
    inline Iterator end() { return m_map.end(); }
    inline ConstIterator end() const { return m_map.end(); }
    inline ConstIterator cend() const { return m_map.cend(); }
  private:
    std::unordered_map<K, V> m_map;
  };

}