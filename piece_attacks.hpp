#ifndef __piece_attacks_hpp
#define __piece_attacks_hpp

#include "bbfunctions.hpp"
#include "bithacks.hpp"
#include "bitfunctions.hpp"
#include "lookup.hpp"
#include <intrin.h>



//ui64 getForwardRayAttacks(ui64 occupied, Dir::EDir dir8, unsigned long square) 
//{
//   ui64 attacks    = lookup::direction_attacks[dir8][square];
//   ui64 blocker    =  attacks & occupied;
//   _BitScanForward64 (&square, blocker | ui64(0x8000000000000000));
//   return attacks ^ lookup::direction_attacks[dir8][square];
//}
//
//ui64 getBackwardRayAttacks(ui64 occupied, Dir::EDir dir8, unsigned long square)
//{
//   ui64 attacks    = lookup::direction_attacks[dir8][square];
//   ui64 blocker    =  attacks & occupied;
//   _BitScanReverse64 (&square, blocker | ui64(1));
//   return attacks ^ lookup::direction_attacks[dir8][square];
//}

 
ui64 getPositiveRayAttacks(ui64 occupied, Dir::EDir dir8, Sq::ESq square) 
{
   ui64 attacks = lookup::direction_attacks[dir8][square];
   ui64 blocker = attacks & occupied;
   if ( blocker ) {
      square = (Sq::ESq)BitScanForward(blocker);
      attacks ^= lookup::direction_attacks[dir8][square];
   }
   return attacks;
}

ui64 getNegativeRayAttacks(ui64 occupied, Dir::EDir dir8, Sq::ESq square) 
{
//	Show<ShowTypes::Console>::Op(occupied);
   ui64 attacks = lookup::direction_attacks[dir8][square];
//   Show<ShowTypes::Console>::Op(attacks);
   ui64 blocker = attacks & occupied;
   if ( blocker ) {
      square = (Sq::ESq)BitScanReverse(blocker);
      attacks ^= lookup::direction_attacks[dir8][square];
   }
   return attacks;
}

inline ui64 GetFrontRayAttacks(ui64 occupied, Dir::EDir dir8, Sq::ESq square) 
{
	return getPositiveRayAttacks(occupied, dir8, square);
}

inline ui64 GetBackRayAttacks(ui64 occupied, Dir::EDir dir8, Sq::ESq square) 
{
	return getNegativeRayAttacks(occupied, dir8, square);
}

inline ui64 BishopAttacks(ui64 occupied, Sq::ESq sq)
{
	ui64 attacks = Constants::clear;
	attacks |= GetFrontRayAttacks( occupied, Dir::NW, sq );
	attacks |= GetFrontRayAttacks( occupied, Dir::NE, sq );
	attacks |= GetBackRayAttacks( occupied, Dir::SW, sq );
	attacks |= GetBackRayAttacks( occupied, Dir::SE, sq );
	return attacks;
}



inline ui64 RookAttacks(ui64 occupied, Sq::ESq sq)
{
	ui64 attacks = Constants::clear;
//	Show<ShowTypes::Console>::Op(attacks);
//	Show<ShowTypes::Console>::Op("--------");
	attacks |= GetFrontRayAttacks( occupied, Dir::N, sq );
//	Show<ShowTypes::Console>::Op(attacks);
//	Show<ShowTypes::Console>::Op("--------");
	attacks |= GetFrontRayAttacks( occupied, Dir::E, sq );
//	Show<ShowTypes::Console>::Op(attacks);
//	Show<ShowTypes::Console>::Op("--------");
	attacks |= GetBackRayAttacks( occupied, Dir::S, sq );
	//Show<ShowTypes::Console>::Op(attacks);
	//Show<ShowTypes::Console>::Op("--------");
	attacks |= GetBackRayAttacks( occupied, Dir::W, sq );
	//Show<ShowTypes::Console>::Op(attacks);
	//Show<ShowTypes::Console>::Op("--------");
	return attacks;
}

inline ui64 QueenAttacks(ui64 occupied, Sq::ESq sq)
{
	return BishopAttacks(occupied, sq) | RookAttacks(occupied, sq);
}

#endif