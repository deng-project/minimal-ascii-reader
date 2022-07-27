/// mar: Minimal ASCII reader for C++
/// licence: Apache, see LICENCE file
/// file: MARAssert.h - Minimal ASCII reader specific assertion macro
/// author: Karl-Mihkel Ott

#ifndef MAR_ASSERT_H
#define MAR_ASSERT_H

#include <cassert>
#include <iostream>

#define MAR_ASSERT(_v) { if(!(_v)) std::cerr << "Assertion failed in '" << __FILE__ << "', line " << __LINE__ << std::endl, std::abort(); }

#endif