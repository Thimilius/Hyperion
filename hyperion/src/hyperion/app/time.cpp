#include "hyppch.hpp"

#include "hyperion/app/time.hpp"

namespace Hyperion {

    float CTime::s_delta_time = 0;
    float CTime::s_fixed_delta_time = 0;
    double CTime::s_time = 0;
    u32 CTime::s_fps = 0;
    double CTime::s_frame_time = 0;

}