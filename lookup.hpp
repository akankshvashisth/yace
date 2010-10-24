#ifndef __lookup_hpp
#define __lookup_hpp

#include "defines.hpp"
#include "typedefs.hpp"
#include "chessutils.hpp"
#include "bbfunctions.hpp"
#include "constants.hpp"
#include "bithacks.hpp"
//#include "show.hpp"

ui64 rankMask(int sq) {return  ui64(0xff) << (sq & 56);}
 
ui64 fileMask(int sq) {return ui64(0x0101010101010101) << (sq & 7);}
 
ui64 diagonalMask(int sq) 
{
   const ui64 maindia = ui64(0x8040201008040201);
   int diag =8*(sq & 7) - (sq & 56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}
 
ui64 antiDiagMask(int sq) 
{
   const ui64 maindia = ui64(0x0102040810204080);
   int diag =56- 8*(sq&7) - (sq&56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}

ui64 rankMaskEx    (int sq) {return (ui64(1) << sq) ^ rankMask(sq);}
ui64 fileMaskEx    (int sq) {return (ui64(1) << sq) ^ fileMask(sq);}
ui64 diagonalMaskEx(int sq) {return (ui64(1) << sq) ^ diagonalMask(sq);}
ui64 antiDiagMaskEx(int sq) {return (ui64(1) << sq) ^ antiDiagMask(sq);}

namespace lookup
{
	static ui64 single_bit_set[64];
    static ui64 not_single_bit_set[64];

	static ui64 knight_moves[64];
	static ui64 king_moves[64];

	static ui64 bishop_moves[64];
	static ui64 rook_moves[64];
	static ui64 queen_moves[64];

	//static ui64 pawn_moves[2][64];
	static ui64 pawn_attacks[2][64];

	static ui64 direction_attacks[8][64];
}


void init_arr_single_bit_set()
{
	for(unsigned i=0; i<64; ++i)
    {
		lookup::single_bit_set[i] = ui64(1)<<i;
        lookup::not_single_bit_set[i] = (~(lookup::single_bit_set[i]));
    }
}

void init_arr_pawn_attacks()
{
	for(unsigned i=0; i<64; ++i)
	{

		//lookup::pawn_moves[0][i] = 
		//lookup::pawn_moves[1][i] = 
		lookup::pawn_attacks[0][i] = WhitePawnAnyAttacks(lookup::single_bit_set[i]);
		lookup::pawn_attacks[1][i] = BlackPawnAnyAttacks(lookup::single_bit_set[i]);
	}
}

void init_arr_king_moves()
{
	for(unsigned i=0; i<64; ++i)
		lookup::king_moves[i] = KingAttacks(lookup::single_bit_set[i]);
}

void init_arr_knight_moves()
{
	for(unsigned i=0; i<64; ++i)
		lookup::knight_moves[i] = KnightAttacks(lookup::single_bit_set[i]);
}

void init_arr_rook_moves()
{
	for(unsigned i=0; i<64; ++i)
		lookup::rook_moves[i] = fileBB[SqFile(Sq::ESq(i))]^rankBB[SqRank(Sq::ESq(i))];
}

void init_arr_bishop_moves()
{
	for(unsigned i=0; i<64; ++i)
	{
		//lookup::bishop_moves[i] = Constants::clear;
		//ui64 ne = lookup::single_bit_set[i], nw = lookup::single_bit_set[i], se = lookup::single_bit_set[i], sw = lookup::single_bit_set[i], temp = Constants::clear;
		//for(unsigned j=0; j<8; ++j)
		//{
		//	ne = NE(ne);
		//	se = SE(se);
		//	nw = NW(nw);
		//	sw = SW(sw);
		//	temp |= (ne|se|nw|sw);
		//}
		//lookup::bishop_moves[i] = temp ;//& (~lookup::single_bit_set[i]);
		lookup::bishop_moves[i] = diagonalMaskEx(i) | antiDiagMaskEx(i);
	}
}

void init_arr_queen_moves()
{
	for(unsigned i=0; i<64; ++i)
	{
		lookup::queen_moves[i] = lookup::rook_moves[i] | lookup::bishop_moves[i]; 
	}
}

void init_arr_direction_attacks()
{
	for(unsigned i=0; i<64; ++i)
		for(unsigned j=0; j<8; ++j)
			lookup::direction_attacks[j][i] = Constants::clear;

	for(unsigned i=0; i<64; ++i)
	{
		ui64 ne = lookup::single_bit_set[i];
		ui64 nw = lookup::single_bit_set[i];
		ui64 se = lookup::single_bit_set[i]; 
		ui64 sw = lookup::single_bit_set[i];
		ui64 n  = lookup::single_bit_set[i];
		ui64 e  = lookup::single_bit_set[i];
		ui64 w  = lookup::single_bit_set[i]; 
		ui64 s  = lookup::single_bit_set[i];
		for(unsigned j=0; j<8; ++j)
		{
			ne = NE(ne);
			se = SE(se);
			nw = NW(nw);
			sw = SW(sw);
			n  =  N(n);
			e  =  E(e);
			w  =  W(w);
			s  =  S(s);
			lookup::direction_attacks[Dir::NE][i] |= ne;
			lookup::direction_attacks[Dir::SE][i] |= se;
			lookup::direction_attacks[Dir::NW][i] |= nw;
			lookup::direction_attacks[Dir::SW][i] |= sw;
			lookup::direction_attacks[Dir::N ][i] |= n ;
			lookup::direction_attacks[Dir::E ][i] |= e ;
			lookup::direction_attacks[Dir::W ][i] |= w ;
			lookup::direction_attacks[Dir::S ][i] |= s ;
		}
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::NE][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::SE][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::NW][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::SW][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::N ][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::E ][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::W ][0]);
		//Show<ShowTypes::Console>::Op("--------");
		//Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::S ][0]);

	}
}



#endif


