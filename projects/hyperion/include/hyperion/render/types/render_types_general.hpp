//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/math/matrix4x4.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class LayerMask : uint32 {
        Nothing = 0,

        Layer1 = BIT(0),
        Layer2 = BIT(1),
        Layer3 = BIT(2),
        Layer4 = BIT(3),
        Layer5 = BIT(4),
        Layer6 = BIT(5),
        Layer7 = BIT(6),
        Layer8 = BIT(7),
        Layer9 = BIT(8),
        Layer10 = BIT(9),
        Layer11 = BIT(10),
        Layer12 = BIT(11),
        Layer13 = BIT(12),
        Layer14 = BIT(13),
        Layer15 = BIT(14),
        Layer16 = BIT(15),
        Layer17 = BIT(16),
        Layer18 = BIT(17),
        Layer19 = BIT(18),
        Layer20 = BIT(19),
        Layer21 = BIT(20),
        Layer22 = BIT(21),
        Layer23 = BIT(22),
        Layer24 = BIT(23),
        Layer25 = BIT(24),
        Layer26 = BIT(25),
        Layer27 = BIT(26),
        Layer28 = BIT(27),
        Layer29 = BIT(28),
        Layer30 = BIT(29),
        Layer31 = BIT(30),

        Default = Layer1,
        Everything = 0xFFFFFFFF,
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(LayerMask);

    enum class ClearFlags {
        None,

        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2),

        All = Color | Depth | Stencil
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearFlags);

    struct CullingParameters {
        Matrix4x4 matrix;
        LayerMask mask;
    };

    struct RenderTargetId {
        AssetId id;

        inline static RenderTargetId Default() { return { 0xFFFFFFFF }; }
    };

}