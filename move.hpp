#ifndef __move_hpp
#define __move_hpp

#include "enums.hpp"
#include "constants.hpp"

struct move_packed;

struct move
{
	PieceType::EPieceType piece;
	PieceType::EPieceType captured;
	PieceType::EPieceType promoted;
	Sq::ESq from, to;
	MoveType::EMoveType type;
	MoveType::ESpecialMoveType special;
};

struct history_elem
{
    Sq::ESq epSq_before_move;
	unsigned fifty_count_before_move;
	bool castling_before_move[CastlingRights::Total];
};

bool operator==( const move& o, const move& a )
{
    if((a.piece != o.piece) || (a.from != o.from) || (a.to != a.to)) return false;
    else if(a.special == MoveType::promotion && a.promoted != o.promoted) return false;
    else if(a.type == MoveType::capture && a.captured != o.captured) return false;
    else return true;
}

/////////////////////////////////

struct move_packed
{
    move_packed( const move& m )
    {
        piece = (unsigned)m.piece;
        captured = (unsigned)m.captured;
        promoted = (unsigned)m.promoted;
        from = (unsigned)m.from;
        to = (unsigned)m.to;
        type = (unsigned)m.type;
        special = (unsigned)m.special;
    }
    move_packed():
        piece   (0),   
        captured(0),
        promoted(0),
        from    (0),
        to      (0),
        type    (0),
        special (0){}

    unsigned piece    : 4;
    unsigned captured : 4;
    unsigned promoted : 4;
    unsigned from     : 6;
    unsigned to       : 6;
    unsigned type     : 2;
    unsigned special  : 3;
};

move MoveFromMovePacked( const move_packed& mp )
{
    move m;
    m.piece = (PieceType::EPieceType)(mp.piece);
    m.captured = (PieceType::EPieceType)(mp.captured);
    m.promoted = (PieceType::EPieceType)(mp.promoted);
    m.from = (Sq::ESq)(mp.from);
    m.to = (Sq::ESq)(mp.to);
    m.type = (MoveType::EMoveType)(mp.type);
    m.special = (MoveType::ESpecialMoveType)(mp.special);
    return m;
}

#endif