#pragma once

#define HYP_CREATE_ENUM_FLAG_OPERATORS(ENUM)                                 \
                                                                             \
inline ENUM operator|(ENUM const lhs, ENUM const rhs)                        \
{                                                                            \
    return static_cast<ENUM>(static_cast<int>(lhs) | static_cast<int>(rhs)); \
}                                                                            \
                                                                             \
inline ENUM operator&(ENUM const lhs, ENUM const rhs)                        \
{                                                                            \
    return static_cast<ENUM>(static_cast<int>(lhs) & static_cast<int>(rhs)); \
}                                                                            \
                                                                             \
inline ENUM operator~(ENUM const rhs)                                        \
{                                                                            \
    return static_cast<ENUM>(~static_cast<int>(rhs));                        \
}                                                                            \
                                                                             \
inline void operator|=(ENUM& lhs, ENUM const rhs)                            \
{                                                                            \
    lhs = static_cast<ENUM>(static_cast<int>(lhs) | static_cast<int>(rhs));  \
}                                                                            \
                                                                             \
inline void operator&=(ENUM& lhs, ENUM const rhs)                            \
{                                                                            \
    lhs = static_cast<ENUM>(static_cast<int>(lhs) & static_cast<int>(rhs));  \
}                                                                            
