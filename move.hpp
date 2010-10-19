#ifndef __move_hpp
#define __move_hpp

#include "enums.hpp"

struct move
{
	PieceType::EPieceType piece;
	PieceType::EPieceType captured;
	PieceType::EPieceType promoted;
	Sq::ESq from, to;
	bool isEp;
	bool isCapture;
	bool isPromotion;
	bool isKingSideCastle;
	bool isQueenSideCastle;
};

#endif