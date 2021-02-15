#pragma once

#ifdef HYP_ENABLE_ASSERTS
    #ifdef HYP_BREAK_ON_ASSERT
        #define HYP_ASSERT_DEBUG_BREAK HYP_DEBUG_BREAK
    #else
        #define HYP_ASSERT_DEBUG_BREAK
    #endif

    #define HYP_ASSERT(x) do {                                                                                                     \
            if (!(x)) {                                                                                                            \
                HYP_LOG_ERROR("Assert", "Assertion failed!\nIn file: {} ({})", __FILE__, __LINE__);                                \
                HYP_ASSERT_DEBUG_BREAK;                                                                                            \
                exit(-1);                                                                                                          \
            } } while (0)
    #define HYP_ASSERT_MESSAGE(x, m) do {                                                                                          \
            if(!(x)) {                                                                                                             \
                HYP_LOG_ERROR("Assert", "Assertion failed!\nIn file: {} ({})\nWith assertion message: {}", __FILE__, __LINE__, m); \
                HYP_ASSERT_DEBUG_BREAK;                                                                                            \
                exit(-1);                                                                                                          \
            } } while(0)

    #define HYP_ASSERT_ENUM_OUT_OF_RANGE HYP_ASSERT_MESSAGE(false, "Enum out of range!")
#else
    #define HYP_ASSERT(x)
    #define HYP_ASSERT_MESSAGE(x, m)
    #define HYP_ASSERT_ENUM_OUT_OF_RANGE
#endif