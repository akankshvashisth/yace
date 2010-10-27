
#ifndef __bitboard_hpp
#define __bitboard_hpp

#include "constants.hpp"
#include "evaluation_constants.hpp"
#include "defines.hpp"
#include "enums.hpp"
#include "typedefs.hpp"
#include "chessutils.hpp"
#include "utility.hpp"
#include "lookup.hpp"
#include "piece_attacks.hpp"
#include "bbfunctions.hpp"
#include "bitfunctions.hpp"

#include "move.hpp"
#include "type_array.hpp"

struct Bitboard;

inline void CopyBitboard( Bitboard& dst, const Bitboard& src );
inline void CopyBitboard( Bitboard* dst, const Bitboard* src );
inline void ClearBitboard( Bitboard* b );

Sq::ESq SquareFromFileRank( Files::EFiles f, Ranks::ERanks r );

struct Bitboard
{
public:
  BB pcBB[16];
  BB emptyBB;
  BB occupiedBB;
  unsigned int halfMoveClock;
  unsigned int fullMoveCounter;
  Sq::ESq epSquare;
  bool castling[CastlingRights::Total];     // WK, WQ, BK, BQ
  bool isWhitesTurn;
 
  type_array<move, Constants::max_game_length> moves;
  type_array<ui64, Constants::max_game_length>  zobrists;
  type_array<move, Constants::max_moves_per_position> moves_arr[Constants::max_depth];
public:
  Bitboard();
  bool operator==( const Bitboard& o ) const;
  bool operator!=(const Bitboard& o ) const;

  bool IsSqEmpty( const Sq::ESq sq ) const;
  bool IsSqOccupied( const Sq::ESq sq ) const;

  BB KingAttacks();

  bool IsKingInCheck();
  bool IsLegal();

  bool IsCastleKingsideLegal();
  bool IsCastleQueensideLegal();

  bool MakeMove( move& m );
  void UnmakeMove();

  bool WhiteCanCastleKingSide() const ;
  bool BlackCanCastleKingSide() const ;
  bool WhiteCanCastleQueenSide() const;
  bool BlackCanCastleQueenSide() const;
  Sq::ESq EpSquare() const;
  void SetEpSquare(Sq::ESq sq);
  bool IsWhitesTurn() const ;
  unsigned HalfMoveClock() const ;
  unsigned FullMoveCounter() const ;
  void IncrementHalfMoveClock() ;
  void IncrementFullMoveCounter() ;

  const ui64& PiecesAt( const PieceType::EPieceType p ) const ;
  PieceType::EPieceType PieceAtSq( const Sq::ESq sq ) const;
  PieceType::EPieceType PieceAtSq_Quick( const Sq::ESq sq, const Color::EColor col ) const;
  void PutPieceAt( const PieceType::EPieceType p, const Sq::ESq sq );
  void ClearPieceAt( const PieceType::EPieceType p, const Sq::ESq sq );
  void ClearPieceAt( const PieceType::EPieceType p, const int file, const int rank );
  void PutPieceAt( const PieceType::EPieceType p, const int file, const int rank );

  void MakeNormalMove_NoUpdate( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to );
  void MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to );
  void MakeCaptureMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to );
  void MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to );
  void MakePromotionMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to );
  void MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to );
  void MakeCapturePromotionMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to );
  void MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to );
  void UpdateWhite();
  void UpdateBlack();
  void UpdateOccupied();
  void UpdateEmpty();
  void UpdateAll();

  bool ASSERT_IsPawnBehindEpCapture( Sq::ESq sq );
  void ASSERTS_MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to );
  void ASSERTS_MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to );
  void ASSERTS_MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to );
  void ASSERTS_MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to );
};


//Try using X_aligned_memcpy_sse2( &dst, &src, sizeof(Bitboard) );
inline void CopyBitboard( Bitboard& dst, const Bitboard& src ){ memcpy( &dst, &src, sizeof(Bitboard) ); }

//Try using X_aligned_memcpy_sse2( dst, src, sizeof(Bitboard) );
inline void CopyBitboard( Bitboard* dst, const Bitboard* src ){ memcpy( dst, src, sizeof(Bitboard) ); }

inline void ClearBitboard( Bitboard* b ){ memset( b, 0, sizeof(Bitboard) ); }

Sq::ESq SquareFromFileRank( Files::EFiles f, Ranks::ERanks r ){ return Sq::ESq((r<<3)+f); }

static Bitboard gBitboard;

////////////////////////////////////////////////////////////////////////////

  Bitboard::Bitboard() { ClearBitboard(this); /*moves.reserve(64);*/ }
  bool Bitboard::operator==( const Bitboard& o ) const
  {
	bool piecesSame = true;
	for(unsigned int i=0; i<16; ++i)
	{
		piecesSame = ( piecesSame && (pcBB[i] == o.pcBB[i]) );
	}
	bool emptySame = emptyBB == o.emptyBB;
	bool occSame = occupiedBB == o.occupiedBB;
	bool hfc = halfMoveClock == o.halfMoveClock;
	bool fmc = fullMoveCounter == o.fullMoveCounter;
	bool eps = epSquare == o.epSquare;
	bool csame = true;
	for(unsigned int i=0; i<4; ++i)
	{
		csame = (csame && ( o.castling[i] == castling[i] ));
	}
	bool wsame = isWhitesTurn == o.isWhitesTurn;
	bool ssame = moves.size() == o.moves.size();
	bool msame = true;
	if(ssame)
	{
		for(unsigned i=0; i<moves.size(); ++i)
		{
			msame = (msame && ( moves[i] == o.moves[i] ));
		}
	}
	return piecesSame && emptySame && occSame && hfc && fmc && eps && csame && wsame && ssame && msame;
  }

  bool Bitboard::operator!=(const Bitboard& o ) const
  {
	  return !(*this == o);
  }



  bool Bitboard::IsSqEmpty( const Sq::ESq sq ) const { return IS_TRUE(SqSetBit(sq) & emptyBB);  }
  bool Bitboard::IsSqOccupied( const Sq::ESq sq ) const { return IS_TRUE(SqSetBit(sq) & occupiedBB); }

  BB Bitboard::KingAttacks()
  {
	  ui64 opPawns, opKnights, opRQ, opBQ;

	  PieceType::EPieceType sideToMove = isWhitesTurn? PieceType::white : PieceType::black;
	  PieceType::EPieceType opponent   = isWhitesTurn? PieceType::black : PieceType::white;

	  const Sq::ESq sqOfKing =  (Sq::ESq)BitScanForward_bsf(pcBB[sideToMove+PieceType::king_diff]);

	  opPawns     = pcBB[opponent+PieceType::pawns_diff];
	  opKnights   = pcBB[opponent+PieceType::knights_diff];
	  opRQ = opBQ = pcBB[opponent+PieceType::queens_diff];
	  opRQ       |= pcBB[opponent+PieceType::rooks_diff];
	  opBQ       |= pcBB[opponent+PieceType::bishops_diff];
	  return (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing] & opPawns) | 
		  	 (lookup::knight_moves[sqOfKing] & opKnights)							 | 
		  	 (BishopAttacks (pcBB[PieceType::all], sqOfKing) & opBQ)				 | 
		  	 (RookAttacks   (pcBB[PieceType::all], sqOfKing) & opRQ);
  }

  bool Bitboard::IsKingInCheck()
  {
	  return KingAttacks() != ui64(0);
  }

  bool Bitboard::IsLegal()
  {
	  ui64 pawns, knights, RQ, BQ, king;

	  PieceType::EPieceType sideToMove = isWhitesTurn? PieceType::white : PieceType::black;
	  PieceType::EPieceType opponent   = isWhitesTurn? PieceType::black : PieceType::white;

	  const Sq::ESq sqOfKing =  (Sq::ESq)BitScanForward_bsf(pcBB[opponent+PieceType::king_diff]);

	  pawns     = pcBB[sideToMove+PieceType::pawns_diff];
	  knights   = pcBB[sideToMove+PieceType::knights_diff];
	  king      = pcBB[sideToMove+PieceType::king_diff];
	  RQ = BQ   = pcBB[sideToMove+PieceType::queens_diff];
	  RQ       |= pcBB[sideToMove+PieceType::rooks_diff];
	  BQ       |= pcBB[sideToMove+PieceType::bishops_diff];
	
	  ui64 p = (lookup::pawn_attacks[opponent/PieceType::black][sqOfKing] & pawns); 
	  ui64 k =  (lookup::king_moves[sqOfKing] & king) ;
	  ui64 n = (lookup::knight_moves[sqOfKing] & knights);
	  ui64 b =  (BishopAttacks (pcBB[PieceType::all], sqOfKing) & BQ);
	  ui64 r =  (RookAttacks   (pcBB[PieceType::all], sqOfKing) & RQ);

	  return !(p || k || n || b || r);
  }

  bool Bitboard::IsCastleKingsideLegal()
  {
	  ui64 pawns, knights, RQ, BQ, king;

	  PieceType::EPieceType sideToMove;
	  PieceType::EPieceType opponent;

	  Sq::ESq sqOfKing0;
	  Sq::ESq sqOfKing1;
	  Sq::ESq sqOfKing2;
      Sq::ESq sqOfRook;
	  CastlingRights::ECastlingRights k,q;

	  if(isWhitesTurn)
	  {
		  sideToMove =  PieceType::white ;
		  opponent   =  PieceType::black ;

		  sqOfKing0 =  Sq::e1 ;
		  sqOfKing1 =  Sq::f1 ;
		  sqOfKing2 =  Sq::g1 ;

          sqOfRook = Sq::h1;

		  k = CastlingRights::WK;
		  q = CastlingRights::WQ;
	  }
	  else
	  {
		  sideToMove =  PieceType::black;
		  opponent   =  PieceType::white;

		  sqOfKing0 =  Sq::e8;
		  sqOfKing1 =  Sq::f8;
		  sqOfKing2 =  Sq::g8;

		  sqOfRook = Sq::h8;
		  
		  k = CastlingRights::BK;
		  q = CastlingRights::BQ;
	  }
    if( (lookup::single_bit_set[sqOfKing0] & pcBB[sideToMove+PieceType::king_diff]) == Constants::clear )
    {
	  castling[k] = false;
	  castling[q] = false;
      return false;
    }
    
    if( (lookup::single_bit_set[sqOfRook] & pcBB[sideToMove+PieceType::rooks_diff]) == Constants::clear )
    {
	  castling[k] = false;
      return false;
    }

    ui64 sqBetweenKingAndRook = lookup::single_bit_set[sqOfKing1] | lookup::single_bit_set[sqOfKing2];
    ui64 sqBetweenKingAndRookNotEmpty = sqBetweenKingAndRook & pcBB[PieceType::all];
    if(sqBetweenKingAndRookNotEmpty) 
    {
      return false;
    }

	  pawns     = pcBB[opponent+PieceType::pawns_diff];
	  knights   = pcBB[opponent+PieceType::knights_diff];
	  RQ = BQ   = pcBB[opponent+PieceType::queens_diff];
	  RQ       |= pcBB[opponent+PieceType::rooks_diff];
	  BQ       |= pcBB[opponent+PieceType::bishops_diff];
	  king      = pcBB[opponent+PieceType::king_diff];

	  ui64 p0 = (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing0] & pawns);
	  ui64 p1 = (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing1] & pawns);
	  ui64 p2 = (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing2] & pawns);
	  ui64 n0 = (lookup::knight_moves[sqOfKing0] & knights);			
	  ui64 n1 = (lookup::knight_moves[sqOfKing1] & knights);			
	  ui64 n2 = (lookup::knight_moves[sqOfKing2] & knights);			
	  ui64 k0 = (lookup::king_moves[sqOfKing0] & king);			
	  ui64 k1 = (lookup::king_moves[sqOfKing1] & king);			
	  ui64 k2 = (lookup::king_moves[sqOfKing2] & king);			
	  ui64 b0 = (BishopAttacks (pcBB[PieceType::all], sqOfKing0) & BQ);			
	  ui64 b1 = (BishopAttacks (pcBB[PieceType::all], sqOfKing1) & BQ);			
	  ui64 b2 = (BishopAttacks (pcBB[PieceType::all], sqOfKing2) & BQ);			
	  ui64 r0 = (RookAttacks   (pcBB[PieceType::all], sqOfKing0) & RQ);				
	  ui64 r1 = (RookAttacks   (pcBB[PieceType::all], sqOfKing1) & RQ);				
	  ui64 r2 = (RookAttacks   (pcBB[PieceType::all], sqOfKing2) & RQ);

	  return !(p0 || p1 || p2 ||
		       n0 || n1 || n2 ||
			   k0 || k1 || k2 ||
			   b0 || b1 || b2 ||
			   r0 || r1 || r2);
  }

  bool Bitboard::IsCastleQueensideLegal()
  {
	  ui64 pawns, knights, RQ, BQ, king;

	  PieceType::EPieceType sideToMove;
	  PieceType::EPieceType opponent;

	  Sq::ESq sqOfKing0;
	  Sq::ESq sqOfKing1;
	  Sq::ESq sqOfKing2;
      Sq::ESq sqOfKing3;
      Sq::ESq sqOfRook;

		  CastlingRights::ECastlingRights k,q;

	  if(isWhitesTurn)
	  {
		  sideToMove =  PieceType::white ;
		  opponent   =  PieceType::black ;

		  sqOfKing0 =  Sq::e1;
		  sqOfKing1 =  Sq::d1;
		  sqOfKing2 =  Sq::c1;
          sqOfKing3 =  Sq::b1;

          sqOfRook = Sq::a1;

		  		  k = CastlingRights::WK;
		  q = CastlingRights::WQ;
	  }
	  else
	  {
		  sideToMove =  PieceType::black ;
		  opponent   =  PieceType::white ;

		  sqOfKing0 =  Sq::e8;
		  sqOfKing1 =  Sq::d8;
		  sqOfKing2 =  Sq::c8;
		  sqOfKing3 =  Sq::b8;

          sqOfRook = Sq::a8;

		  		  k = CastlingRights::BK;
		  q = CastlingRights::BQ;
	  }

    if( (lookup::single_bit_set[sqOfKing0] & pcBB[sideToMove+PieceType::king_diff]) == Constants::clear )
    {
	  castling[k] = false;
	  castling[q] = false;
      return false;
    }
    
    if( (lookup::single_bit_set[sqOfRook] & pcBB[sideToMove+PieceType::rooks_diff]) == Constants::clear )
    {
	  castling[q] = false;
      return false;
    }

    ui64 sqBetweenKingAndRook = lookup::single_bit_set[sqOfKing1] | lookup::single_bit_set[sqOfKing2] | lookup::single_bit_set[sqOfKing3];
    ui64 sqBetweenKingAndRookNotEmpty = sqBetweenKingAndRook & pcBB[PieceType::all];
    if(sqBetweenKingAndRookNotEmpty) 
    {
      return false;
    }

	  pawns     = pcBB[opponent+PieceType::pawns_diff];
	  knights   = pcBB[opponent+PieceType::knights_diff];
	  RQ = BQ   = pcBB[opponent+PieceType::queens_diff];
	  RQ       |= pcBB[opponent+PieceType::rooks_diff];
	  BQ       |= pcBB[opponent+PieceType::bishops_diff];
	  king      = pcBB[opponent+PieceType::king_diff];

	  ui64 p0 = (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing0] & pawns)	;
	  ui64 p1 = (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing1] & pawns)	;
	  ui64 p2 = (lookup::pawn_attacks[sideToMove/PieceType::black][sqOfKing2] & pawns)	;
	  ui64 n0 = (lookup::knight_moves[sqOfKing0] & knights)				;			
	  ui64 n1 = (lookup::knight_moves[sqOfKing1] & knights)				;			
	  ui64 n2 = (lookup::knight_moves[sqOfKing2] & knights)				;			
	  ui64 k0 = (lookup::king_moves[sqOfKing0] & king)					;			
	  ui64 k1 = (lookup::king_moves[sqOfKing1] & king)					;			
	  ui64 k2 = (lookup::king_moves[sqOfKing2] & king)					;			
	  ui64 b0 = (BishopAttacks (pcBB[PieceType::all], sqOfKing0) & BQ)	;			
	  ui64 b1 = (BishopAttacks (pcBB[PieceType::all], sqOfKing1) & BQ)	;			
	  ui64 b2 = (BishopAttacks (pcBB[PieceType::all], sqOfKing2) & BQ)	;			
	  ui64 r0 = (RookAttacks   (pcBB[PieceType::all], sqOfKing0) & RQ);				
	  ui64 r1 = (RookAttacks   (pcBB[PieceType::all], sqOfKing1) & RQ);				
	  ui64 r2 = (RookAttacks   (pcBB[PieceType::all], sqOfKing2) & RQ);

	  return !(p0 || p1 || p2 ||
		       n0 || n1 || n2 ||
			   k0 || k1 || k2 ||
			   b0 || b1 || b2 ||
			   r0 || r1 || r2);
  }

  bool Bitboard::ASSERT_IsPawnBehindEpCapture( Sq::ESq sq )
  {
	return isWhitesTurn ? ((lookup::single_bit_set[ (sq)-8 ] & pcBB[PieceType::bpawns]) != Constants::clear) :
						            ((lookup::single_bit_set[ (sq)+8 ] & pcBB[PieceType::wpawns]) != Constants::clear);
  }

  bool Bitboard::MakeMove( move& m )
  {
	m.fifty_count_before_move = halfMoveClock;
	m.castling_before_move[0] = castling[0];
	m.castling_before_move[1] = castling[1];
	m.castling_before_move[2] = castling[2];
	m.castling_before_move[3] = castling[3];
	m.epSq_before_move = epSquare;

	epSquare = Sq::none;
    
    switch( m.special )
    {
    case MoveType::ep:
      {
		    assert( m.piece == PieceType::wpawns || PieceType::bpawns );
		    assert( (((int)m.to - (int)m.from) == 7)  || (((int)m.from - (int)m.to) == 9) ||
				        (((int)m.to - (int)m.from) == -7) || (((int)m.from - (int)m.to) == -9) );
		    assert( ASSERT_IsPawnBehindEpCapture(m.to) ); 

		    MakeNormalMove_NoUpdate(m.piece, m.from, m.to);
		    if(IsWhitesTurn())
		    {
			    pcBB[PieceType::bpawns] &= (~(S(lookup::single_bit_set[m.to])));
		    }
		    else
		    {
			    pcBB[PieceType::wpawns] &= (~(N(lookup::single_bit_set[m.to])));
		    }
			halfMoveClock = 0;
			moves.push_back(m);
        break;
      }
    case MoveType::promotion:
      {
        switch( m.type )
        {
        case MoveType::capture:
            MakeCapturePromotionMove_NoUpdate(m.piece, m.captured, m.promoted, m.from, m.to);
			break;
        default: //MoveType::normal
            MakePromotionMove_NoUpdate(m.piece, m.promoted, m.from, m.to);
        }
		halfMoveClock = 0;
		moves.push_back(m);
        break;
      }
    case MoveType::castle_kingside:
	  {
		  bool isLegal = true;//IsCastleKingsideLegal();
		  if(isWhitesTurn)
		  {
			  ClearPieceAt(PieceType::wking, SqFile(Sq::e1),  SqRank(Sq::e1) );
				PutPieceAt  (PieceType::wking, SqFile(Sq::g1), SqRank(Sq::g1) );
				ClearPieceAt(PieceType::wrooks, SqFile(Sq::h1), SqRank(Sq::h1) );
				PutPieceAt  (PieceType::wrooks, SqFile(Sq::f1), SqRank(Sq::f1) );
        castling[CastlingRights::WK] = false;
			  castling[CastlingRights::WQ] = false;
		  }
		  else
		  {
			  ClearPieceAt(PieceType::bking, SqFile(Sq::e8), SqRank(Sq::e8) );
				PutPieceAt  (PieceType::bking, SqFile(Sq::g8), SqRank(Sq::g8) );
				ClearPieceAt(PieceType::brooks, SqFile(Sq::h8), SqRank(Sq::h8) );
				PutPieceAt  (PieceType::brooks, SqFile(Sq::f8), SqRank(Sq::f8) );
        castling[CastlingRights::BK] = false;
			  castling[CastlingRights::BQ] = false;
				++fullMoveCounter;
		  }
		  halfMoveClock = 0;
			isWhitesTurn = !isWhitesTurn;

			if(isLegal) UpdateAll();
			moves.push_back(m);
		  return isLegal;
	  }
    case MoveType::castle_queenside:
    {
		  bool isLegal = true;//IsCastleQueensideLegal();
		  if(isWhitesTurn)
		  {
			  ClearPieceAt(PieceType::wking, SqFile(Sq::e1), SqRank(Sq::e1) );
				PutPieceAt  (PieceType::wking, SqFile(Sq::c1), SqRank(Sq::c1) );
				ClearPieceAt(PieceType::wrooks, SqFile(Sq::a1), SqRank(Sq::a1) );
				PutPieceAt  (PieceType::wrooks, SqFile(Sq::d1), SqRank(Sq::d1) );
        castling[CastlingRights::WK] = false;
			  castling[CastlingRights::WQ] = false;
		  }
		  else
		  {
			  ClearPieceAt(PieceType::bking, SqFile(Sq::e8), SqRank(Sq::e8) );
				PutPieceAt  (PieceType::bking, SqFile(Sq::c8), SqRank(Sq::c8) );
				ClearPieceAt(PieceType::brooks, SqFile(Sq::a8), SqRank(Sq::a8) );
				PutPieceAt  (PieceType::brooks, SqFile(Sq::d8), SqRank(Sq::d8) );
        castling[CastlingRights::BK] = false;
			  castling[CastlingRights::BQ] = false;
				++fullMoveCounter;
		  }
		  halfMoveClock = 0;

		  isWhitesTurn = !isWhitesTurn;
		  if(isLegal) UpdateAll();
		  moves.push_back(m);
		  return isLegal;
	  }
    default:
      {
        switch( m.type )
        {
        case MoveType::capture:
            MakeCaptureMove_NoUpdate(m.piece, m.captured, m.from, m.to);
			halfMoveClock = 0;
			moves.push_back(m);
            break;
        default:
            MakeNormalMove_NoUpdate(m.piece, m.from, m.to);
            if(m.piece == PieceType::wpawns && m.to != (m.from+8))
		    {
			    epSquare = Sq::ESq((unsigned)m.from+8);
		    }
		    else if(m.piece == PieceType::bpawns && m.to != (m.from-8))
		    {
			    epSquare = Sq::ESq((unsigned)m.from-8);
		    }
			moves.push_back(m);
        }
      }
    }
	if(!isWhitesTurn)
	{
		++fullMoveCounter;
	}
	
	  isWhitesTurn = !isWhitesTurn;

	  UpdateAll();

    if(castling[CastlingRights::WK] || castling[CastlingRights::WQ]) 
	  {
		  if((PiecesAt(PieceType::wking) & lookup::single_bit_set[Sq::e1])==Constants::clear)
		  {
			castling[CastlingRights::WK] = false;
			castling[CastlingRights::WQ] = false;
		  }
		  if((PiecesAt(PieceType::wrooks) & lookup::single_bit_set[Sq::h1])==Constants::clear)
		  {
			castling[CastlingRights::WK] = false;
		  }
		  if((PiecesAt(PieceType::wrooks) & lookup::single_bit_set[Sq::a1])==Constants::clear)
		  {
			castling[CastlingRights::WQ] = false;
		  }
    }
    if(castling[CastlingRights::BK] || castling[CastlingRights::BQ])
    {
      if((PiecesAt(PieceType::bking) & lookup::single_bit_set[Sq::e8])==Constants::clear)
		  {
			castling[CastlingRights::BK] = false;
			castling[CastlingRights::BQ] = false;
		  }
		  if((PiecesAt(PieceType::brooks) & lookup::single_bit_set[Sq::h8])==Constants::clear)
		  {
			castling[CastlingRights::BK] = false;
		  }
		  if((PiecesAt(PieceType::brooks) & lookup::single_bit_set[Sq::a8])==Constants::clear)
		  {
			castling[CastlingRights::BQ] = false;
		  }
	  }

	  return IsLegal();
  }

  void Bitboard::UnmakeMove()
  {
	  move& m = moves.back();
	  
	  halfMoveClock = m.fifty_count_before_move;
	  epSquare      = m.epSq_before_move;
	  castling[0]   = m.castling_before_move[0];
	  castling[1]   = m.castling_before_move[1];
	  castling[2]   = m.castling_before_move[2];
	  castling[3]   = m.castling_before_move[3];

	  switch(m.special)
	  {
	  case MoveType::ep:
		  {
			MakeNormalMove_NoUpdate(m.piece, m.to, m.from);
			if(IsWhitesTurn())
		    {
			    pcBB[PieceType::wpawns] |= (N(lookup::single_bit_set[m.to]));
				--fullMoveCounter;
		    }
		    else
		    {
			    pcBB[PieceType::bpawns] |= (S(lookup::single_bit_set[m.to]));
		    }
		  }
		  break;
	  case MoveType::promotion:
		  {
			  switch(m.type)
			  {
			  case MoveType::capture:
				  PutPieceAt(m.piece, SqFile(m.from), SqRank(m.from));
				  PutPieceAt(m.captured, SqFile(m.to), SqRank(m.to));
				  ClearPieceAt(m.promoted, SqFile(m.to), SqRank(m.to));
				  break;
			  default:
				  PutPieceAt(m.piece, SqFile(m.from), SqRank(m.from));
				  ClearPieceAt(m.promoted, SqFile(m.to), SqRank(m.to));
			  }
			  if(IsWhitesTurn()) --fullMoveCounter;
		  }
		  break;
	  case MoveType::castle_kingside:
		  {
			  if(isWhitesTurn)
			  {
				  //ClearPieceAt(PieceType::bking, SqFile(Sq::g8) );
				  //PutPieceAt  (PieceType::bking, SqFile(Sq::e8 );
				  //ClearPieceAt(PieceType::brooks, SqFile(Sq::f8 );
				  //PutPieceAt  (PieceType::brooks, SqFile(Sq::h8 );
				  MakeNormalMove_NoUpdate(PieceType::bking, Sq::g8, Sq::e8);
				  MakeNormalMove_NoUpdate(PieceType::brooks, Sq::f8, Sq::h8);
				  --fullMoveCounter;
			  }
			  else
			  {
				  //ClearPieceAt(PieceType::wking, SqFile(Sq::g1 );
				  //PutPieceAt  (PieceType::wking, SqFile(Sq::e1 );
				  //ClearPieceAt(PieceType::wrooks, SqFile(Sq::f1 );
				  //PutPieceAt  (PieceType::wrooks, SqFile(Sq::h1 );
				  MakeNormalMove_NoUpdate(PieceType::wking, Sq::g1, Sq::e1);
				  MakeNormalMove_NoUpdate(PieceType::wrooks, Sq::f1, Sq::h1);
			  }
		  }
		  break;
	  case MoveType::castle_queenside:
		  {
			  if(isWhitesTurn)
			  {
				  //ClearPieceAt(PieceType::bking, SqFile(Sq::c8 );
				  //PutPieceAt  (PieceType::bking, SqFile(Sq::e8 );
				  //ClearPieceAt(PieceType::brooks, SqFile(Sq::d8  );
				  //PutPieceAt  (PieceType::brooks, SqFile(Sq::a8 );
				  MakeNormalMove_NoUpdate(PieceType::bking, Sq::c8, Sq::e8);
				  MakeNormalMove_NoUpdate(PieceType::brooks, Sq::d8, Sq::a8);
				  --fullMoveCounter;
			  }
			  else
			  {
				  //ClearPieceAt(PieceType::wking, SqFile(Sq::c1 );
				  //PutPieceAt  (PieceType::wking, Sq::e1 );
				  //ClearPieceAt(PieceType::wrooks, Sq::d1  );
				  //PutPieceAt  (PieceType::wrooks, Sq::a1 );
				  MakeNormalMove_NoUpdate(PieceType::wking, Sq::c1, Sq::e1);
				  MakeNormalMove_NoUpdate(PieceType::wrooks, Sq::d1, Sq::a1);
			  }
		  }
		  break;
	  default:
		  switch(m.type)
		  {
		  case MoveType::capture:
			  MakeNormalMove_NoUpdate(m.piece, m.to, m.from);
			  PutPieceAt(m.captured, m.to);
			  break;
		  default:
			  MakeNormalMove_NoUpdate(m.piece, m.to, m.from);
		  }
		  if(IsWhitesTurn()) --fullMoveCounter;
	  }
	  isWhitesTurn = !isWhitesTurn;
	  moves.pop_back();
	  UpdateAll();
  }

  bool Bitboard::WhiteCanCastleKingSide() const { return castling[CastlingRights::WK]; }
  bool Bitboard::BlackCanCastleKingSide() const { return castling[CastlingRights::BK]; }
  bool Bitboard::WhiteCanCastleQueenSide() const { return castling[CastlingRights::WQ]; }
  bool Bitboard::BlackCanCastleQueenSide() const { return castling[CastlingRights::BQ]; }

  Sq::ESq Bitboard::EpSquare() const { return (epSquare); }
  void Bitboard::SetEpSquare(Sq::ESq sq)
  {
    epSquare = sq;
  }

  bool Bitboard::IsWhitesTurn() const { return isWhitesTurn; }

  unsigned Bitboard::HalfMoveClock() const { return halfMoveClock; }
  unsigned Bitboard::FullMoveCounter() const { return fullMoveCounter; }
  void Bitboard::IncrementHalfMoveClock() { ++halfMoveClock; }
  void Bitboard::IncrementFullMoveCounter() { ++fullMoveCounter; }

  const ui64& Bitboard::PiecesAt( const PieceType::EPieceType p ) const { return pcBB[p]; }

  PieceType::EPieceType Bitboard::PieceAtSq_Quick( const Sq::ESq sq, const Color::EColor col ) const
  {
    //assumes there is a piece at the square!!
    const BB square = SqSetBit(sq);
    switch(col)
    {
    case Color::white:
      for( unsigned i=Ranges::wBegin; i<Ranges::wEnd; ++i )
      {
        if( IS_TRUE(square & pcBB[i]) ) return (PieceType::EPieceType)i;
      }
      break;
    default:
      for( unsigned i=Ranges::bBegin; i<Ranges::bEnd; ++i )
      {
        if( IS_TRUE(square & pcBB[i]) ) return (PieceType::EPieceType)i;
      }     
    };
    return PieceType::none;
  }

  PieceType::EPieceType Bitboard::PieceAtSq( const Sq::ESq sq ) const
  {
    const BB square = SqSetBit(sq);
    if( IS_TRUE(square & emptyBB) )
    {
      return PieceType::none;
    }
    else
    {
      if( IS_TRUE(square & pcBB[PieceType::white] ))
      {
        for( unsigned i=Ranges::wBegin; i<Ranges::wEnd; ++i )
        {
          if( IS_TRUE(square & pcBB[i]) ) return (PieceType::EPieceType)i;
        }
      }
      else
      {
        for( unsigned i=Ranges::bBegin; i<Ranges::bEnd; ++i )
        {
          if( IS_TRUE(square & pcBB[i]) ) return (PieceType::EPieceType)i;
        }
      }
    }
    return PieceType::none;
  }
  void Bitboard::PutPieceAt( const PieceType::EPieceType p, const Sq::ESq sq )
  {
    PutPieceAt( p, SqFile(sq), SqRank(sq) );
    UpdateAll();
  }
  void Bitboard::ClearPieceAt( const PieceType::EPieceType p, const Sq::ESq sq )
  {
    ClearPieceAt( p, SqFile(sq), SqRank(sq) );
    UpdateAll();
  }
  void Bitboard::ClearPieceAt( const PieceType::EPieceType p, const int file, const int rank )
  {
    pcBB[p] &= (~(fileBB[file] & rankBB[rank]));
  }
  void Bitboard::PutPieceAt( const PieceType::EPieceType p, const int file, const int rank )
  {
    pcBB[p] |= ( fileBB[file] & rankBB[rank] );
  }
  void Bitboard::MakeNormalMove_NoUpdate( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeNormalMove(p,from,to) );

    ClearPieceAt(p, SqFile(from), SqRank(from) );
    PutPieceAt(p,  SqFile(to), SqRank(to) );
  }
  void Bitboard::MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    MakeNormalMove_NoUpdate(p,from,to);
    UpdateAll();
  }
  void Bitboard::MakeCaptureMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeCaptureMove(p,c,from,to) );

    const int toFile = SqFile(to);
    const int toRank = SqRank(to);

    ClearPieceAt(p, SqFile(from), SqRank(from));
    ClearPieceAt(c, toFile, toRank);
    PutPieceAt(p, toFile, toRank);
  }
  void Bitboard::MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
  {
	MakeCaptureMove_NoUpdate(p,c,from,to);
    UpdateAll();
  }
  void Bitboard::MakePromotionMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakePromotionMove(p,n,from,to) ) ;

    ClearPieceAt(p, SqFile(from), SqRank(from));
    PutPieceAt(n,  SqFile(to), SqRank(to) );
  }
  void Bitboard::MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    MakePromotionMove_NoUpdate(p, n, from, to);
    UpdateAll();
  }
  void Bitboard::MakeCapturePromotionMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeCapturePromotionMove(p,c,n,from,to) );

    const int toFile = SqFile(to);
    const int toRank = SqRank(to);

    ClearPieceAt(p, SqFile(from), SqRank(from));
    ClearPieceAt(c, toFile, toRank);
    PutPieceAt(n, toFile, toRank);
    //UpdateAll();
  }
  void Bitboard::MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    MakeCapturePromotionMove_NoUpdate(p,c,n,from,to);
    UpdateAll();
  }
  void Bitboard::UpdateWhite()
  {
    pcBB[PieceType::white] = Constants::clear;
    for( unsigned i=Ranges::wBegin; i<Ranges::wEnd; ++i )
    {
      assert( (pcBB[PieceType::white] & pcBB[i]) == Constants::clear );
      pcBB[PieceType::white] |= pcBB[i];
    }
  }
  void Bitboard::UpdateBlack()
  {
    pcBB[PieceType::black] = Constants::clear;
    for( unsigned i=Ranges::bBegin; i<Ranges::bEnd; ++i )
    {
      assert( (pcBB[PieceType::black] & pcBB[i]) == Constants::clear );
      pcBB[PieceType::black] |= pcBB[i];
    }
  }
  void Bitboard::UpdateOccupied()
  {
    assert( (pcBB[PieceType::black] & pcBB[PieceType::white]) == Constants::clear ); //No overlap
    DO_SAFE(occupiedBB = Constants::clear);
    occupiedBB = (pcBB[PieceType::black] | pcBB[PieceType::white]);
	pcBB[PieceType::all] = occupiedBB;
  }
  void Bitboard::UpdateEmpty()
  {
    DO_SAFE(emptyBB = Constants::clear);
    emptyBB = ~occupiedBB;
	pcBB[PieceType::none] = emptyBB;
  }
  void Bitboard::UpdateAll()
  {
    UpdateWhite();
    UpdateBlack();
    UpdateOccupied();
    UpdateEmpty();
  }

  void Bitboard::ASSERTS_MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    assert( IsSqOccupied(to) );
    assert( IsSqOccupied(from) );
    assert( PieceColor(p) != PieceColor(c) );
    assert( PieceColor(p) != Color::none );
    assert( PieceColor(c) != Color::none );
    assert( !IsKing(c) );
    assert( !IsPawn(c) );
    assert( PieceAtSq(from) == p );
    assert( PieceAtSq(to) == c );
    assert( IsPawn(p) );
    assert( If_IsTrue( (PieceColor(p)==Color::white), (SqSetBit(to)&Constants::rank_8) ) );
    assert( If_IsTrue( (PieceColor(p)==Color::black), (SqSetBit(to)&Constants::rank_1) ) );
    assert( !IsPawn(n) );
    assert( !IsKing(n) );
  }
  void Bitboard::ASSERTS_MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    assert( IsSqOccupied(from) );
    assert( IsPawn(p) );
    assert( If_IsTrue( (PieceColor(p)==Color::white), (SqSetBit(to)&Constants::rank_8) ) );
    assert( If_IsTrue( (PieceColor(p)==Color::black), (SqSetBit(to)&Constants::rank_1) ) );
    assert( !IsPawn(n) );
    assert( !IsKing(n) );
	PieceType::EPieceType pas = PieceAtSq(from);
    assert( pas == p );
  }
  void Bitboard::ASSERTS_MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
  {
    assert( IsSqOccupied(to) );
    assert( IsSqOccupied(from) );
    assert( PieceColor(p) != PieceColor(c) );
    assert( PieceColor(p) != Color::none );
    assert( PieceColor(c) != Color::none );
    assert( !IsKing(c) );
    assert( PieceAtSq(from) == p );
    assert( PieceAtSq(to) == c );
  }
  void Bitboard::ASSERTS_MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    assert( IsSqEmpty(to) );
    assert( IsSqOccupied(from) );
    assert( PieceAtSq(from) == p );
  }

///////////////////////////////////////////////////////////////////////////

#endif



