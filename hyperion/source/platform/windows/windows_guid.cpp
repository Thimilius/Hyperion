//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_guid.hpp"

//---------------------- Library Includes ----------------------
#include <objbase.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  String WindowsGuid::ToString() const {
    GUID new_guid_native;
    new_guid_native.Data1 = static_cast<unsigned long>(m_data[0] >> 32);
    new_guid_native.Data2 = static_cast<unsigned short>(m_data[0] >> 16);
    new_guid_native.Data3 = static_cast<unsigned short>(m_data[0]);
    new_guid_native.Data4[0] = static_cast<unsigned char>(m_data[1] >> 56);
    new_guid_native.Data4[1] = static_cast<unsigned char>(m_data[1] >> 48);
    new_guid_native.Data4[2] = static_cast<unsigned char>(m_data[1] >> 40);
    new_guid_native.Data4[3] = static_cast<unsigned char>(m_data[1] >> 32);
    new_guid_native.Data4[4] = static_cast<unsigned char>(m_data[1] >> 24);
    new_guid_native.Data4[5] = static_cast<unsigned char>(m_data[1] >> 16);
    new_guid_native.Data4[6] = static_cast<unsigned char>(m_data[1] >> 8);
    new_guid_native.Data4[7] = static_cast<unsigned char>(m_data[1]);

    WideString wide_string;
    wide_string.resize(39);
    StringFromGUID2(new_guid_native, wide_string.data(), 39);

    return StringUtils::Utf16ToUtf8(wide_string);
  }

  //--------------------------------------------------------------
  bool8 WindowsGuid::operator==(const WindowsGuid &other) const {
    return m_data[0] == other.m_data[0] && m_data[1] == other.m_data[1];
  }

  //--------------------------------------------------------------
  bool8 WindowsGuid::operator!=(const WindowsGuid &other) const {
    return !(*this == other);
  }

  //--------------------------------------------------------------
  WindowsGuid WindowsGuid::Generate() {
    GUID guid_native;
    CoCreateGuid(&guid_native);

    WindowsGuid guid;
    guid.m_data[0] = (static_cast<uint64>(guid_native.Data1) << 32) | (static_cast<uint64>(guid_native.Data2) << 16) | (static_cast<uint64>(guid_native.Data3));
    guid.m_data[1] = (static_cast<uint64>(guid_native.Data4[0]) << 56) |
      (static_cast<uint64>(guid_native.Data4[1]) << 48) |
      (static_cast<uint64>(guid_native.Data4[2]) << 40) |
      (static_cast<uint64>(guid_native.Data4[3]) << 32) |
      (static_cast<uint64>(guid_native.Data4[4]) << 24) |
      (static_cast<uint64>(guid_native.Data4[5]) << 16) |
      (static_cast<uint64>(guid_native.Data4[6]) << 8) |
      (static_cast<uint64>(guid_native.Data4[7]));

    return guid;
  }

  //--------------------------------------------------------------
  WindowsGuid WindowsGuid::Generate(const String &string) {
    GUID guid_native;
    WideString wide_string = StringUtils::Utf8ToUtf16(string);
    IIDFromString(wide_string.c_str(), &guid_native);

    WindowsGuid guid;
    guid.m_data[0] = (static_cast<uint64>(guid_native.Data1) << 32) | (static_cast<uint64>(guid_native.Data2) << 16) | (static_cast<uint64>(guid_native.Data3));
    guid.m_data[1] = (static_cast<uint64>(guid_native.Data4[0]) << 56) |
      (static_cast<uint64>(guid_native.Data4[1]) << 48) |
      (static_cast<uint64>(guid_native.Data4[2]) << 40) |
      (static_cast<uint64>(guid_native.Data4[3]) << 32) |
      (static_cast<uint64>(guid_native.Data4[4]) << 24) |
      (static_cast<uint64>(guid_native.Data4[5]) << 16) |
      (static_cast<uint64>(guid_native.Data4[6]) << 8) |
      (static_cast<uint64>(guid_native.Data4[7]));

    return guid;
  }

}
