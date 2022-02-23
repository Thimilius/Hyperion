//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class DataPool {
  public:
    inline DataPool(uint64 element_size, uint64 element_count) : m_element_size(element_size) { m_data = new byte[element_size * element_count]; }
    inline ~DataPool() { delete[] m_data; }
  public:
    inline void *Get(uint64 index) { return m_data + index * m_element_size; }
  private:
    byte *m_data;
    uint64 m_element_size;
  };

}