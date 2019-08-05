#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include <stb/stb_image.h>

#include <hyperion/app/application.hpp>