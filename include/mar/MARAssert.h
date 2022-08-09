/// mar: Minimal ASCII reader for C++
/// licence: Apache, see LICENCE file
/// file: MARAssert.h - Minimal ASCII reader specific assertion macro
/// author: Karl-Mihkel Ott

#ifndef MAR_ASSERT_H
#define MAR_ASSERT_H

#include <iostream>

#ifdef _DEBUG
    #include <cassert>
    #define MAR_ASSERT(_val) assert(_val)
#else
    #define MAR_ASSERT(_val)
#endif

#endif
