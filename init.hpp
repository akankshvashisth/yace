#ifndef __init_hpp
#define __init_hpp

#include "evaluation_constants.hpp"
#include "lookup.hpp"
#include "bithacks.hpp"
#include "random.hpp"
#include "zobrist_rand.hpp"

void init_all()
{
    initpopCountOfShorts65536();
    initpopCountOfByte256();
    InitRandom();
    init_zobrist();

	init_arr_single_bit_set();
	init_arr_king_moves();
	init_arr_knight_moves();
	init_arr_rook_moves();
	init_arr_bishop_moves();
	init_arr_queen_moves();
	init_arr_direction_attacks();
	init_arr_pawn_attacks();

    init_eval_constants();

}

#endif


