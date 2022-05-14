include("build/debug/conanbuildinfo.premake.lua")
package_assimp_debug_libdirs = { conan_libdirs_assimp, conan_libdirs_irrxml, conan_libdirs_kuba_zip, conan_libdirs_minizip, conan_libdirs_poly2tri, conan_libdirs_rapidjson, conan_libdirs_utfcpp }
package_assimp_debug_links = { conan_libs_assimp, conan_libs_irrxml, conan_libs_kuba_zip, conan_libs_minizip, conan_libs_poly2tri, conan_libs_rapidjson, conan_libs_utfcpp, conan_libs_zlib }
package_bullet_debug_libdirs = conan_libdirs_bullet3
package_bullet_debug_links = conan_libs_bullet3
package_freetype_debug_libdirs = { conan_libdirs_freetype, conan_libdirs_libpng, conan_libdirs_bzip2, conan_libdirs_brotli, conan_libdirs_zlib }
package_freetype_debug_links = { conan_libs_freetype, conan_libs_libpng, conan_libs_bzip2, conan_libs_brotli, conan_libs_zlib }
package_rttr_debug_libdirs = conan_libdirs_rttr
package_rttr_debug_links = conan_libs_rttr
package_shaderc_debug_libdirs = { conan_libdirs_shaderc, conan_libdirs_glslang, conan_libdirs_spirv_tools, conan_libdirs_spirv_headers }
package_shaderc_debug_links = { conan_libs_shaderc, conan_libs_glslang, conan_libs_spirv_tools, conan_libs_spirv_headers }
package_yaml_debug_libdirs = conan_libdirs_yaml_cpp
package_yaml_debug_links = conan_libs_yaml_cpp

include("build/release/conanbuildinfo.premake.lua")
package_assimp_release_libdirs = { conan_libdirs_assimp, conan_libdirs_irrxml, conan_libdirs_kuba_zip, conan_libdirs_minizip, conan_libdirs_poly2tri, conan_libdirs_rapidjson, conan_libdirs_utfcpp }
package_assimp_release_links = { conan_libs_assimp, conan_libs_irrxml, conan_libs_kuba_zip, conan_libs_minizip, conan_libs_poly2tri, conan_libs_rapidjson, conan_libs_utfcpp, conan_libs_zlib }
package_bullet_release_libdirs = conan_libdirs_bullet3
package_bullet_release_links = conan_libs_bullet3
package_freetype_release_libdirs = { conan_libdirs_freetype, conan_libdirs_libpng, conan_libdirs_bzip2, conan_libdirs_brotli, conan_libdirs_zlib }
package_freetype_release_links = { conan_libs_freetype, conan_libs_libpng, conan_libs_bzip2, conan_libs_brotli, conan_libs_zlib }
package_rttr_release_libdirs = conan_libdirs_rttr
package_rttr_release_links = conan_libs_rttr
package_shaderc_release_libdirs = { conan_libdirs_shaderc, conan_libdirs_glslang, conan_libdirs_spirv_tools, conan_libdirs_spirv_headers }
package_shaderc_release_links = { conan_libs_shaderc, conan_libs_glslang, conan_libs_spirv_tools, conan_libs_spirv_headers }
package_yaml_release_libdirs = conan_libdirs_yaml_cpp
package_yaml_release_links = conan_libs_yaml_cpp

package_assimp_includedirs = { conan_includedirs_assimp, conan_includedirs_irrxml, conan_includedirs_kuba_zip, conan_includedirs_minizip, conan_includedirs_poly2tri, conan_includedirs_rapidjson, conan_includedirs_utfcpp }
package_bullet_includedirs = conan_includedirs_bullet3
package_freetype_includedirs = { conan_includedirs_freetype, conan_includedirs_libpng, conan_includedirs_bzip2, conan_includedirs_brotli, conan_includedirs_zlib }
package_stb_includedirs = conan_includedirs_stb
package_rttr_includedirs = conan_includedirs_rttr
package_shaderc_includedirs = { conan_includedirs_shaderc, conan_includedirs_glslang, conan_includedirs_spirv_tools, conan_includedirs_spirv_headers }
package_vulkan_memory_allocator_includedirs = conan_includedirs_vulkan_memory_allocator
package_yaml_includedirs = conan_includedirs_yaml_cpp