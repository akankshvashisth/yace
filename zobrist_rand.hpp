#ifndef __zobrist_rand_hpp
#define __zobrist_rand_hpp

#include "enums.hpp"
#include "typedefs.hpp"
#include "random.hpp"

namespace Zobrist
{
    static ui64 zobrist[PieceType::none][Sq::none];
    static ui64 black_move_key;
    static ui64 castling_key[CastlingRights::Total]/* = { 1<<0,1<<8,1<<16,1<<24 }*/;
    static ui64 ep_key[Sq::none];
}

void init_zobrist();

void init_zobrist()
{
    for(unsigned i=0; i<PieceType::none; ++i)
        for(unsigned k=0; k<Sq::none; ++k)
        {
            Zobrist::zobrist[i][k] = Random<ui64>();
        }

    for(unsigned i=0; i<CastlingRights::Total; ++i)
        Zobrist::castling_key[i] = Random<ui64>();

    for(unsigned k=0; k<Sq::none; ++k)
        Zobrist::ep_key[k] = Random<ui64>(); 

    Zobrist::black_move_key = Random<ui64>();
}



#endif
