#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {

    class Guid {
    public:
        String ToString() const;

        bool operator==(const Guid &other) const;
        bool operator!=(const Guid &other) const;
    public:
        static Guid Create();
        static Guid Create(const String &string);
    private:
        uint64 data[2] = { 0 };
    private:
        friend struct std::hash<Hyperion::Guid>;
    };

}

namespace std {

    template <>
    struct hash<Hyperion::Guid> {
        std::size_t operator()(const Hyperion::Guid &guid) const {
            return (hash<uint64>()(guid.data[0]) ^ (hash<uint64>()(guid.data[1]) << 1)) >> 1;
        }
    };

}