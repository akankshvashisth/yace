#ifndef __typedefs_hpp
#define __typedefs_hpp


#ifdef HAVE_BOOST
#include <boost/cstdint.hpp>

typedef boost::uint8_t  ui08;
typedef boost::uint16_t ui16;
typedef boost::uint32_t ui32;
typedef boost::uint64_t ui64;
typedef boost::int8_t  i08;
typedef boost::int16_t i16;
typedef boost::int32_t i32;
typedef boost::int64_t i64;
typedef ui64 BB; //bitboard
typedef ui64 C64;
#else

#include <cstdint>

typedef uint8_t  ui08;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;
typedef int8_t  i08;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ui64 BB; //bitboard
typedef ui64 C64;
#endif
#endif

