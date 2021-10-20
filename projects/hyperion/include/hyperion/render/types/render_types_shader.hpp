#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/render/types/render_types_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    using ShaderPropertyId = uint64;

    enum class ShaderStageFlags {
        Unknown,

        Vertex = BIT(0),
        Fragment = BIT(1),
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

    enum class ShaderRenderOrder {
        Unknown,

        Opaque,
        Transparent
    };
    
    enum class ShaderCullingMode {
        Unknown,

        Off,
        Front,
        Back,
    };

    enum class ShaderBlendingMode {
        Unknown,

        On,
        Off,
    };

    enum class ShaderZWrite {
        Unknown,

        On,
        Off
    };

    struct ShaderAttributes {
        ShaderRenderOrder render_order = ShaderRenderOrder::Opaque;

        ShaderCullingMode culling_mode = ShaderCullingMode::Back;
        // TODO: Properly add ability to specify blending mode.
        ShaderBlendingMode blending_mode = ShaderBlendingMode::Off;
        ShaderZWrite z_write = ShaderZWrite::On;
    };

    enum class ShaderPropertyType {
        Unknown,

        Float,
        Int,
        Vector,
        Color,
        Matrix,

        Texture
    };

    union ShaderPropertyStorage {
        float32 float32;
        int32 int32;
        Vector4 vector4;
        Color color;
        Matrix4x4 matrix4x4;

        struct Texture {
            AssetId id = AssetInfo::INVALID_ID;
            TextureDimension dimension = TextureDimension::Texture2D;
            uint32 render_texture_attchment_index = 0;
        } texture;
    };

    struct ShaderProperty {
        String name;

        ShaderPropertyId id;
        ShaderPropertyType type;
        ShaderPropertyStorage storage = { };

        inline static ShaderPropertyId PropertyToId(const String &name) { return std::hash<String>{}(name); }
    };

    using ShaderPropertyCollection = Array<ShaderProperty>;

    struct ShaderData {
        ShaderAttributes attributes;
        ShaderPropertyCollection properties;

        ShaderStageFlags stage_flags;

        String vertex_source;
        String fragment_source;
    };

}