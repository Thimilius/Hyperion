#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CAsset {
    private:
        TString m_name;
    public:
        CAsset() { }
        CAsset(const TString &name) : m_name(name) {  }

        inline const TString &GetName() const { return m_name; }
        inline void SetName(const TString &name) { m_name = name; }
    };

}