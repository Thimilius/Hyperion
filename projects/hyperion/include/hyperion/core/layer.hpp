#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class LayerMask : u32 {
        Nothing = 0,

        Layer1 = BIT(1),
        Layer2 = BIT(2),
        Layer3 = BIT(3),
        Layer4 = BIT(4),
        Layer5 = BIT(5),
        Layer6 = BIT(6),
        Layer7 = BIT(7),
        Layer8 = BIT(8),
        Layer9 = BIT(9),
        Layer10 = BIT(10),
        Layer11 = BIT(11),
        Layer12 = BIT(12),
        Layer13 = BIT(13),
        Layer14 = BIT(14),
        Layer15 = BIT(15),
        Layer16 = BIT(16),
        Layer17 = BIT(17),
        Layer18 = BIT(18),
        Layer19 = BIT(19),
        Layer20 = BIT(20),
        Layer21 = BIT(21),
        Layer22 = BIT(22),
        Layer23 = BIT(23),
        Layer24 = BIT(24),
        Layer25 = BIT(25),
        Layer26 = BIT(26),
        Layer27 = BIT(27),
        Layer28 = BIT(28),
        Layer29 = BIT(29),
        Layer30 = BIT(30),
        Layer31 = BIT(31),

        Default = Layer1,
        Everything = 0xFFFFFFFF,
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(LayerMask);

}