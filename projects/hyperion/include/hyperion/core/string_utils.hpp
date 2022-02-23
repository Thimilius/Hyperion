//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <format>
#include <string>

//---------------------- Project Includes ----------------------
#include "hyperion/core/collections/array.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using String = std::string;
  using WideString = std::wstring;

  class StringUtils {
  public:
    template<typename ...Args>
    inline static String Format(const char *fmt, Args... args) {
      return std::format(fmt, args...);
    }

    template<typename ...Args>
    inline static String Format(const String &fmt, Args... args) {
      return std::format(fmt, args...);
    }

    inline static bool8 EndsWith(String const &string, String const &ending) {
      if (string.length() >= ending.length()) {
        return string.compare(string.length() - ending.length(), ending.length(), ending) == 0;
      } else {
        return false;
      }
    }

    inline static String GetExtension(const String &string) {
      size_t position = string.find_last_of('.');
      if (position != string.npos) {
        return string.substr(position);
      }
      return string;
    }

    inline static constexpr uint32 Hash(const char *input) {
      return *input ? static_cast<uint32>(*input) + 33 * Hash(input + 1) : 5381;
    }

    static Array<uint32> GetCodepointsFromUtf8(const String &string);
    static String GetUtf8FromCodepoint(uint32 codepoint);
    static String GetUtf8FromCodepoints(const Array<uint32> &codepoints);
    static uint32 GetLastUtf8CodepointSize(const String &string);

    static WideString Utf8ToUtf16(const String &string);
    static String Utf16ToUtf8(const WideString &string);
  private:
    StringUtils() = delete;
    ~StringUtils() = delete;
  };

}