#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Asset {
    private:
        String m_name;
    public:
        Asset() { }
        Asset(const String &name) : m_name(name) {  }

        inline const String &GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }
    };

}