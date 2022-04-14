//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderBuffer {
  public:
    RenderBuffer() = default;
    RenderBuffer(uint64 size) { m_data.Resize(size); }
  public:
    inline const Array<byte> &GetData() const { return m_data; }
    inline Array<byte> &GetData() { return m_data; }
    inline void SetData(const Array<byte> &data) { m_data = data; }
  private:
    Array<byte> m_data;
  };

}
