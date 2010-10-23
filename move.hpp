#ifndef __move_hpp
#define __move_hpp

#include "enums.hpp"
#include "constants.hpp"
#include <cassert>

struct move
{
	PieceType::EPieceType piece;
	PieceType::EPieceType captured;
	PieceType::EPieceType promoted;
	Sq::ESq from, to, epSq_before_move;
	MoveType::EMoveType type;
	MoveType::ESpecialMoveType special;
	unsigned fifty_count_before_move;
	bool castling_before_move[CastlingRights::Total];
};

template< unsigned N >
struct move_array
{
	const move& back() const { return mvs[msize-1]; }
	      move& back()       { return mvs[msize-1]; }
    void push_back( const move& m )
	{  
		mvs[msize] = m; 
		++msize; 
		assert(msize < N);
	}
	unsigned size() const { return msize; }
	void pop_back()
	{
		--msize;
		assert(msize >= 0);
	}
	const move& operator[]( unsigned i ) const { return mvs[i]; }
	      move& operator[]( unsigned i )       { return mvs[i]; }
	void clear()
	{
		msize = 0;
	}
	move mvs[N];
	unsigned int msize;
};

bool operator==( const move& o, const move& a )
{
	return (a.piece == o.piece) &&
		(a.captured == o.captured) &&
		(a.promoted == o.promoted) &&
		(a.from == o.from) &&
		(a.to == o.to) &&
		(a.epSq_before_move == o.epSq_before_move) &&
		(a.type == o.type) &&
		(a.special == o.special) &&
		(a.fifty_count_before_move == o.fifty_count_before_move) &&
		(a.castling_before_move[0] == o.castling_before_move[0]) &&
		(a.castling_before_move[1] == o.castling_before_move[1]) &&
		(a.castling_before_move[2] == o.castling_before_move[2]) &&
		(a.castling_before_move[3] == o.castling_before_move[3]);
}

#endif