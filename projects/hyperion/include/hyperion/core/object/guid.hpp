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
    };

}