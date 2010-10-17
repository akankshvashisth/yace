#ifndef __constants_hpp
#define __constants_hpp

#include "typedefs.hpp"

namespace Constants
{
  typedef ui64 EConstants;

  static const ui64 clear             = 0ULL;
  static const ui64 all               = 0xffffffffffffffffULL;

  static const ui64 file_a            = 0x0101010101010101ULL;
  static const ui64 file_b            = 0x0202020202020202ULL;
  static const ui64 file_c            = 0x0404040404040404ULL;
  static const ui64 file_d            = 0x0808080808080808ULL;
  static const ui64 file_e            = 0x1010101010101010ULL;
  static const ui64 file_f            = 0x2020202020202020ULL;
  static const ui64 file_g            = 0x4040404040404040ULL;
  static const ui64 file_h            = 0x8080808080808080ULL;

  static const ui64 rank_1            = 0x00000000000000FFULL;
  static const ui64 rank_2            = 0x000000000000FF00ULL;
  static const ui64 rank_3            = 0x0000000000FF0000ULL;
  static const ui64 rank_4            = 0x00000000FF000000ULL;
  static const ui64 rank_5            = 0x000000FF00000000ULL;
  static const ui64 rank_6            = 0x0000FF0000000000ULL;
  static const ui64 rank_7            = 0x00FF000000000000ULL;
  static const ui64 rank_8            = 0xFF00000000000000ULL;

  static const ui64 main_diagonal     = 0x8040201008040201ULL;
  static const ui64 main_antidiagonal = 0x0102040810204080ULL;

  static const ui64 all_light_squares = 0x55AA55AA55AA55AAULL;
  static const ui64 all_dark_squares  = 0xAA55AA55AA55AA55ULL;

  static const ui64 not_file_a        = ~file_a;
  static const ui64 not_file_b        = ~file_b;
  static const ui64 not_file_c        = ~file_c;
  static const ui64 not_file_d        = ~file_d;
  static const ui64 not_file_e        = ~file_e;
  static const ui64 not_file_f        = ~file_f;
  static const ui64 not_file_g        = ~file_g;
  static const ui64 not_file_h        = ~file_h;

  static const ui64 not_rank_1        = ~rank_1;
  static const ui64 not_rank_2        = ~rank_2;
  static const ui64 not_rank_3        = ~rank_3;
  static const ui64 not_rank_4        = ~rank_4;
  static const ui64 not_rank_5        = ~rank_5;
  static const ui64 not_rank_6        = ~rank_6;
  static const ui64 not_rank_7        = ~rank_7;
  static const ui64 not_rank_8        = ~rank_8;

  static const ui64 not_file_a_or_b   = ~(file_a|file_b);
  static const ui64 not_file_g_or_h   = ~(file_g|file_h);

  static const ui64 file_abc          = 0x0707070707070707ULL;
  static const ui64 file_fgh          = 0xE0E0E0E0E0E0E0E0ULL;
  static const ui64 file_bcd          = 0x0E0E0E0E0E0E0E0EULL;
  static const ui64 file_efg          = 0x7070707070707070ULL;

  static const ui64 not_main_diagonal     = ~main_diagonal;
  static const ui64 not_main_antidiagonal = ~main_antidiagonal;
}

const ui64 rankBB[] =
{
  Constants::rank_1,
  Constants::rank_2,
  Constants::rank_3,
  Constants::rank_4,
  Constants::rank_5,
  Constants::rank_6,
  Constants::rank_7,
  Constants::rank_8
};

const ui64 fileBB[] =
{
  Constants::file_a,
  Constants::file_b,
  Constants::file_c,
  Constants::file_d,
  Constants::file_e,
  Constants::file_f,
  Constants::file_g,
  Constants::file_h
};

const i64 directionShift[]={ 7, 8, 9, -1, 1 -9, -8, -7 };

#endif
