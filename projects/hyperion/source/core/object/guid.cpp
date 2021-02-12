//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/object/guid.hpp"

//---------------------- Library Includes ----------------------
#ifdef HYP_PLATFORM_WINDOWS
#include <objbase.h>
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    String Guid::ToString() const {
#ifdef HYP_PLATFORM_WINDOWS
        GUID new_guid_native;
        new_guid_native.Data1 = static_cast<unsigned long>(data[0] >> 32);
        new_guid_native.Data2 = static_cast<unsigned short>(data[0] >> 16);
        new_guid_native.Data3 = static_cast<unsigned short>(data[0]);
        new_guid_native.Data4[0] = static_cast<unsigned char>(data[1] >> 56);
        new_guid_native.Data4[1] = static_cast<unsigned char>(data[1] >> 48);
        new_guid_native.Data4[2] = static_cast<unsigned char>(data[1] >> 40);
        new_guid_native.Data4[3] = static_cast<unsigned char>(data[1] >> 32);
        new_guid_native.Data4[4] = static_cast<unsigned char>(data[1] >> 24);
        new_guid_native.Data4[5] = static_cast<unsigned char>(data[1] >> 16);
        new_guid_native.Data4[6] = static_cast<unsigned char>(data[1] >> 8);
        new_guid_native.Data4[7] = static_cast<unsigned char>(data[1]);

        WideString wide_string;
        wide_string.resize(39);
        StringFromGUID2(new_guid_native, wide_string.data(), 39);

        return StringUtils::Utf16ToUtf8(wide_string);
#endif
    }

    //--------------------------------------------------------------
    bool Guid::operator==(const Guid &other) const {
        return data[0] == other.data[0] && data[1] == other.data[1];
    }

    //--------------------------------------------------------------
    bool Guid::operator!=(const Guid &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    Guid Guid::Create() {
#ifdef HYP_PLATFORM_WINDOWS
        GUID guid_native;
        CoCreateGuid(&guid_native);

        Guid guid;
        guid.data[0] = (static_cast<uint64>(guid_native.Data1) << 32) | (static_cast<uint64>(guid_native.Data2) << 16) | (static_cast<uint64>(guid_native.Data3));
        guid.data[1] = (static_cast<uint64>(guid_native.Data4[0]) << 56) |
            (static_cast<uint64>(guid_native.Data4[1]) << 48) |
            (static_cast<uint64>(guid_native.Data4[2]) << 40) |
            (static_cast<uint64>(guid_native.Data4[3]) << 32) |
            (static_cast<uint64>(guid_native.Data4[4]) << 24) |
            (static_cast<uint64>(guid_native.Data4[5]) << 16) |
            (static_cast<uint64>(guid_native.Data4[6]) << 8) |
            (static_cast<uint64>(guid_native.Data4[7]));

        return guid;
#endif
    }

    //--------------------------------------------------------------
    Guid Guid::Create(const String &string) {
#ifdef HYP_PLATFORM_WINDOWS
        GUID guid_native;
        WideString wide_string = StringUtils::Utf8ToUtf16(string);
        IIDFromString(wide_string.c_str(), &guid_native);

        Guid guid;
        guid.data[0] = (static_cast<uint64>(guid_native.Data1) << 32) | (static_cast<uint64>(guid_native.Data2) << 16) | (static_cast<uint64>(guid_native.Data3));
        guid.data[1] = (static_cast<uint64>(guid_native.Data4[0]) << 56) |
            (static_cast<uint64>(guid_native.Data4[1]) << 48) |
            (static_cast<uint64>(guid_native.Data4[2]) << 40) |
            (static_cast<uint64>(guid_native.Data4[3]) << 32) |
            (static_cast<uint64>(guid_native.Data4[4]) << 24) |
            (static_cast<uint64>(guid_native.Data4[5]) << 16) |
            (static_cast<uint64>(guid_native.Data4[6]) << 8) |
            (static_cast<uint64>(guid_native.Data4[7]));

        return guid;
#endif
    }

}