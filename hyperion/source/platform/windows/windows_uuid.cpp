//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_uuid.hpp"

//---------------------- Library Includes ----------------------
#include <objbase.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  String WindowsUUID::ToString() const {
    GUID new_uuid_native;
    new_uuid_native.Data1 = static_cast<unsigned long>(m_data[0] >> 32);
    new_uuid_native.Data2 = static_cast<unsigned short>(m_data[0] >> 16);
    new_uuid_native.Data3 = static_cast<unsigned short>(m_data[0]);
    new_uuid_native.Data4[0] = static_cast<unsigned char>(m_data[1] >> 56);
    new_uuid_native.Data4[1] = static_cast<unsigned char>(m_data[1] >> 48);
    new_uuid_native.Data4[2] = static_cast<unsigned char>(m_data[1] >> 40);
    new_uuid_native.Data4[3] = static_cast<unsigned char>(m_data[1] >> 32);
    new_uuid_native.Data4[4] = static_cast<unsigned char>(m_data[1] >> 24);
    new_uuid_native.Data4[5] = static_cast<unsigned char>(m_data[1] >> 16);
    new_uuid_native.Data4[6] = static_cast<unsigned char>(m_data[1] >> 8);
    new_uuid_native.Data4[7] = static_cast<unsigned char>(m_data[1]);

    WideString wide_string;
    wide_string.resize(39);
    StringFromGUID2(new_uuid_native, wide_string.data(), 39);

    return StringUtils::Utf16ToUtf8(wide_string);
  }

  //--------------------------------------------------------------
  bool8 WindowsUUID::operator==(const WindowsUUID &other) const {
    return m_data[0] == other.m_data[0] && m_data[1] == other.m_data[1];
  }

  //--------------------------------------------------------------
  bool8 WindowsUUID::operator!=(const WindowsUUID &other) const {
    return !(*this == other);
  }

  //--------------------------------------------------------------
  WindowsUUID WindowsUUID::Generate() {
    GUID uuid_native;
    CoCreateGuid(&uuid_native);

    WindowsUUID uuid;
    uuid.m_data[0] = (static_cast<uint64>(uuid_native.Data1) << 32) | (static_cast<uint64>(uuid_native.Data2) << 16) | (static_cast<uint64>(uuid_native.Data3));
    uuid.m_data[1] = (static_cast<uint64>(uuid_native.Data4[0]) << 56) |
      (static_cast<uint64>(uuid_native.Data4[1]) << 48) |
      (static_cast<uint64>(uuid_native.Data4[2]) << 40) |
      (static_cast<uint64>(uuid_native.Data4[3]) << 32) |
      (static_cast<uint64>(uuid_native.Data4[4]) << 24) |
      (static_cast<uint64>(uuid_native.Data4[5]) << 16) |
      (static_cast<uint64>(uuid_native.Data4[6]) << 8) |
      (static_cast<uint64>(uuid_native.Data4[7]));

    return uuid;
  }

  //--------------------------------------------------------------
  WindowsUUID WindowsUUID::Generate(const String &string) {
    GUID uuid_native;
    WideString wide_string = StringUtils::Utf8ToUtf16(string);
    IIDFromString(wide_string.c_str(), &uuid_native);

    WindowsUUID uuid;
    uuid.m_data[0] = (static_cast<uint64>(uuid_native.Data1) << 32) | (static_cast<uint64>(uuid_native.Data2) << 16) | (static_cast<uint64>(uuid_native.Data3));
    uuid.m_data[1] = (static_cast<uint64>(uuid_native.Data4[0]) << 56) |
      (static_cast<uint64>(uuid_native.Data4[1]) << 48) |
      (static_cast<uint64>(uuid_native.Data4[2]) << 40) |
      (static_cast<uint64>(uuid_native.Data4[3]) << 32) |
      (static_cast<uint64>(uuid_native.Data4[4]) << 24) |
      (static_cast<uint64>(uuid_native.Data4[5]) << 16) |
      (static_cast<uint64>(uuid_native.Data4[6]) << 8) |
      (static_cast<uint64>(uuid_native.Data4[7]));

    return uuid;
  }

}
