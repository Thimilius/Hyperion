#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Guid {
    public:
        String ToString() const;

        bool operator==(const Guid &other) const;
        bool operator!=(const Guid &other) const;

        static Guid Create();
        static Guid Create(const String &string);
    private:
        uint64 data[2];
    };

}