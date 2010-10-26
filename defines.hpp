#ifndef __defines_hpp
#define __defines_hpp

#include <cassert>
#include "utility.hpp"

#define DO_EVERYTHING_SAFELY

#ifdef DO_EVERYTHING_SAFELY
#define DO_SAFE(p)  (p) 
#define IS_TRUE(p)  (IsTrue(p))
#define IS_FALSE(p) (IsFalse(p))
#else
#define DO_SAFE(p)
#define IS_TRUE(p) (p)
#define IS_FALSE(p) (p)
#endif

#endif