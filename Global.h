#pragma once

#include <iostream>
#include <string>
#include <memory>

using c8 = char;
using c16 = short;
using c32 = int;
using c64 = long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long;

using s8 = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long;

using f32 = float;
using f64 = double;

using std::string;
using std::shared_ptr;
using std::unique_ptr;

template<typename _Type>
concept TFundamental = std::is_fundamental_v<_Type>;

template<typename _Type>
concept TClass = std::is_class_v<_Type>;

template<typename _Type>
concept TPointer = std::is_pointer_v<_Type>;
