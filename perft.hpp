#ifndef __perft_hpp
#define __perft_hpp

#include "move_gen.hpp"
#include <cassert>
#include <vector>

struct PerftHelper
{
    PerftHelper():
    mates               (0), 
    captures            (0), 
    count               (0), 
    ep                  (0), 
    stalemates          (0), 
    promotions          (0), 
    castle_kingsides    (0), 
    castle_queensides   (0)
    {}
    ui64 mates, 
         captures, 
         count, 
         ep, 
         stalemates, 
         promotions, 
         castle_kingsides, 
         castle_queensides;
};

static ui64 PerftWithZobTest( Bitboard& bb, int depth, int current_depth, PerftHelper& ph )
{
    int n_moves, i;
    ui64 nodes = 0;

    if (depth == 0){ return 1; }

    GeneratePseudoLegalMoves(bb, bb.moves_arr[current_depth]);
    n_moves = bb.moves_arr[current_depth].size();

    for (i = 0; i < n_moves; i++) 
    {
        if(bb.MakeMove(bb.moves_arr[current_depth][i]))
        {
            bb.zobrists[current_depth+1] = bb.zobrists[current_depth];
            UpdateZobristFromMove(bb.zobrists[current_depth+1], bb.moves_arr[current_depth][i], bb);
            move& m = bb.moves_arr[current_depth][i];
            ui64 zobNow = ZobristFromBitboard(bb);
            ui64 zobUp =  bb.zobrists[current_depth+1];
            assert(zobNow != zobUp);
			nodes += PerftWithZobTest(bb,depth - 1, current_depth + 1, ph);
        }
        bb.UnmakeMove();
    }
    return nodes;
}

static ui64 PerftWithZobUpdate( Bitboard& bb, int depth, int current_depth, PerftHelper& ph )
{
    int n_moves, i;
    ui64 nodes = 0;

    if (depth == 0){ return 1; }

    GeneratePseudoLegalMoves(bb, bb.moves_arr[current_depth]);
    n_moves = bb.moves_arr[current_depth].size();

    for (i = 0; i < n_moves; i++) 
    {
        if(bb.MakeMove(bb.moves_arr[current_depth][i]))
        {
            bb.zobrists[current_depth+1] = bb.zobrists[current_depth];
            UpdateZobristFromMove(bb.zobrists[current_depth+1], bb.moves_arr[current_depth][i], bb);
            nodes += PerftWithZobUpdate(bb,depth - 1, current_depth + 1, ph);
        }
        bb.UnmakeMove();
    }
    return nodes;
}

static ui64 Perft( Bitboard& bb, int depth, int current_depth, PerftHelper& ph )
{
    int n_moves, i;
    ui64 nodes = 0;

    if (depth == 0){ return 1; }

    GeneratePseudoLegalMoves(bb, bb.moves_arr[current_depth]);
    n_moves = bb.moves_arr[current_depth].size();

    for (i = 0; i < n_moves; i++) 
    {
        if(bb.MakeMove(bb.moves_arr[current_depth][i]))
        {
            nodes += Perft(bb,depth - 1, current_depth + 1, ph);
        }
        bb.UnmakeMove();
    }
    return nodes;
}

static std::vector< std::pair<move, ui64> > Divide( Bitboard& bb, int depth )
{
    type_array<move, Constants::max_moves_per_position> mvs;
    std::vector< std::pair<move, ui64> > toReturn;
    int n_moves, i;
    ui64 nodes = 0;

    PerftHelper ph;
    mvs = GeneratePseudoLegalMoves(bb, mvs);
    n_moves = mvs.size();
    for (i = 0; i < n_moves; i++) 
    {
        Bitboard bb_local(bb);
        move m = mvs[i];
        if(bb_local.MakeMove(m))
        {
            toReturn.push_back(std::make_pair(m,Perft(bb_local,depth - 1, 0, ph)));
        }
    }
    return toReturn;
}


#endif
