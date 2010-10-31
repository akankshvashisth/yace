#ifndef __move_hpp
#define __move_hpp

#include "enums.hpp"
#include "constants.hpp"

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
	return  (a.piece == o.piece) &&
		    (a.captured == o.captured) &&
		    (a.promoted == o.promoted) &&
		    (a.from == o.from) &&
		    (a.to == o.to) &&
		    //(a.epSq_before_move == o.epSq_before_move) &&
		    (a.type == o.type) &&
		    (a.special == o.special); //&&
		    //(a.fifty_count_before_move == o.fifty_count_before_move) &&
		    //(a.castling_before_move[0] == o.castling_before_move[0]) &&
		    //(a.castling_before_move[1] == o.castling_before_move[1]) &&
		    //(a.castling_before_move[2] == o.castling_before_move[2]) &&
		    //(a.castling_before_move[3] == o.castling_before_move[3]);
}

/////////////////////////////////

struct move_packed
{
  unsigned char data[6];
};


#endif