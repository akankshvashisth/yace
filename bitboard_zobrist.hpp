#ifndef __bitboard_zobrist_hpp
#define __bitboard_zobrist_hpp

#include "bitboard.hpp"
#include "bitfunctions.hpp"
#include "move.hpp"
#include "zobrist_rand.hpp"

inline void xorWithZobristForPieceTypeAndBitboard( Bitboard& b, ui64& zob, PieceType::EPieceType t )
{
    ui64  p = b.PiecesAt(t);
    const ui64 p_c = PopulationCount(p);
    for(unsigned i=0; i<p_c; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(p);
		p &= (lookup::not_single_bit_set[sq]);
        zob ^= Zobrist::zobrist[t][sq];
    }
}

ui64 ZobristFromBitboard( Bitboard&  b )
{
    ui64 zob = 0;

    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::wpawns);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::wknights);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::wbishops);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::wrooks);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::wqueens);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::wking);

    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::bpawns);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::bknights);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::bbishops);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::brooks);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::bqueens);
    xorWithZobristForPieceTypeAndBitboard(b, zob, PieceType::bking);


    for(unsigned i=0; i<CastlingRights::Total; ++i)
    {
        if(b.castling[i])
        {
            zob ^= Zobrist::castling_key[i];
        }
    }

    if( b.EpSquare() != Sq::none )
    {
        zob ^= Zobrist::ep_key[b.EpSquare()]; 
    }

    if(b.IsWhitesTurn())
    { 
        zob ^= Zobrist::black_move_key; 
    }

    return zob;
}

void UpdateZobristFromMove( ui64& zob, move& m, Bitboard& b )
{
    for(unsigned i=0; i<CastlingRights::Total; ++i)
    {
        if( b.castling[i] != m.castling_before_move[i] )
        {
            zob ^= Zobrist::castling_key[i];
        }
    }

    if(m.epSq_before_move != Sq::none)
    {
        zob ^= Zobrist::ep_key[m.epSq_before_move];
    }
    if(b.EpSquare() != Sq::none)
    {
        zob ^= Zobrist::ep_key[b.EpSquare()];
    }

    zob ^= Zobrist::zobrist[m.piece][m.from]; //remove piece from location

    zob ^= Zobrist::black_move_key; //update side to move.

    switch(m.special)
    {
    case MoveType::promotion:
        {
            switch(m.type)
            {
            case MoveType::capture:
                {
                    zob ^= Zobrist::zobrist[m.captured][m.to]; //remove captured piece.
                    zob ^= Zobrist::zobrist[m.promoted][m.to]; // put promoted piece in its place.
                }
            default:
                {
                    zob ^= Zobrist::zobrist[m.promoted][m.to]; // put promoted piece in its place.
                }
            }
        }
        break;
    case MoveType::ep:
        {
            assert((m.epSq_before_move & Constants::rank_3)||(m.epSq_before_move & Constants::rank_6));
                
            zob ^= Zobrist::zobrist[m.piece][m.to]; //put the pawn at the ep square.

            if(m.epSq_before_move & Constants::rank_3)
            {
                zob ^= Zobrist::zobrist[PieceType::wpawns][unsigned(m.to)+8]; //remove captured pawn.
            }
            else //rank_6
            {
                zob ^= Zobrist::zobrist[PieceType::bpawns][unsigned(m.to)-8]; //remove captured pawn.
            }
        }
        break;
    case MoveType::castle_kingside:
        {
            zob ^= Zobrist::zobrist[m.piece][unsigned(m.from)+2]; //Put king at castled square.

            PieceType::EPieceType rook = PieceType::EPieceType((unsigned)(m.piece)-2); //get type of rook (wrooks/brooks)
            
            zob ^= Zobrist::zobrist[rook][unsigned(m.from)+3]; //remove rook from original position
            zob ^= Zobrist::zobrist[rook][unsigned(m.from)+1]; //put rook at castled position
        }
        break;
    case MoveType::castle_queenside:
        {
            zob ^= Zobrist::zobrist[m.piece][unsigned(m.from)-2]; //Put king at castled square.

            PieceType::EPieceType rook = PieceType::EPieceType((unsigned)(m.piece)-2); //get type of rook (wrooks/brooks)
            
            zob ^= Zobrist::zobrist[rook][unsigned(m.from)-4]; //remove rook from original position
            zob ^= Zobrist::zobrist[rook][unsigned(m.from)-1]; //put rook at castled position
        }
        break;
    default:
        switch(m.type)
        {
        case MoveType::capture:
            {
                zob ^= Zobrist::zobrist[m.captured][m.to]; //remove captured piece
                zob ^= Zobrist::zobrist[m.piece][m.to]; //put the capturing piece in its place
            }
        default:
            {
                zob ^= Zobrist::zobrist[m.piece][m.to]; //put the moving place in place.
            }
        }
    }

}




#endif
