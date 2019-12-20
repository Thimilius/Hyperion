#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class LayerMask : unsigned int {
        None    = 0,
        Layer1  = BIT(0),
        Layer2  = BIT(1),
        Layer3  = BIT(2),
        Layer4  = BIT(3),
        Layer5  = BIT(4),
        Layer6  = BIT(5),
        Layer7  = BIT(6),
        Layer8  = BIT(7),
        Layer9  = BIT(8),
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
        Layer32 = BIT(31)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(LayerMask);

    struct Layer {

    };

}