
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
#include "move.hpp"

#include <cstring>

struct Bitboard;

inline void CopyBitboard( Bitboard& dst, const Bitboard& src );
inline void CopyBitboard( Bitboard* dst, const Bitboard* src );
inline void ClearBitboard( Bitboard* b );

Sq::ESq SquareFromFileRank( Files::EFiles f, Ranks::ERanks r )
{
  return Sq::ESq((r<<3)+f);
}

struct Bitboard
{
public:
  BB pcBB[16];
  BB emptyBB;
  BB occupiedBB;
	unsigned int halfMoveClock;
	unsigned int fullMoveCounter;
  Sq::ESq epSquare;
  bool castling[4];     // WK, WQ, BK, BQ
  bool isWhitesTurn;
public:
  Bitboard() { ClearBitboard(this); }
  Bitboard( const Bitboard& other ) { CopyBitboard( this, &other ); }

  bool IsSqEmpty( const Sq::ESq sq ) const { return IS_TRUE(SqSetBit(sq) & emptyBB);  }
  bool IsSqOccupied( const Sq::ESq sq ) const { return IS_TRUE(SqSetBit(sq) & occupiedBB); }

  BB KingAttacks()
  {
	  ui64 opPawns, opKnights, opRQ, opBQ;

	  PieceType::EPieceType sideToMove = isWhitesTurn? PieceType::white : PieceType::black;
	  PieceType::EPieceType opponent   = isWhitesTurn? PieceType::black : PieceType::white;

	  const Sq::ESq sqOfKing =  (Sq::ESq)BitScanForward(pcBB[sideToMove+PieceType::king_diff]);

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

  bool IsKingInCheck()
  {
	  return KingAttacks() != ui64(0);
  }

  bool IsLegal()
  {
	  ui64 pawns, knights, RQ, BQ;

	  PieceType::EPieceType sideToMove = isWhitesTurn? PieceType::white : PieceType::black;
	  PieceType::EPieceType opponent   = isWhitesTurn? PieceType::black : PieceType::white;

	  const Sq::ESq sqOfKing =  (Sq::ESq)BitScanForward(pcBB[opponent+PieceType::king_diff]);

	  pawns     = pcBB[sideToMove+PieceType::pawns_diff];
	  knights   = pcBB[sideToMove+PieceType::knights_diff];
	  RQ = BQ   = pcBB[sideToMove+PieceType::queens_diff];
	  RQ       |= pcBB[sideToMove+PieceType::rooks_diff];
	  BQ       |= pcBB[sideToMove+PieceType::bishops_diff];
	  return ((lookup::pawn_attacks[opponent/PieceType::black][sqOfKing] & pawns)   | 
		  	 (lookup::knight_moves[sqOfKing] & knights)						        | 
		  	 (BishopAttacks (pcBB[PieceType::all], sqOfKing) & BQ)				    | 
			 (RookAttacks   (pcBB[PieceType::all], sqOfKing) & RQ)) == Constants::clear;
  }

  bool MakeMove( move m )
  {
	  epSquare = Sq::none;
	  if(m.isEp)
	  {
		  MakeNormalMove_NoUpdate(m.piece, m.from, m.to);
		  if(IsWhitesTurn())
		  {
			  pcBB[PieceType::bpawns] &= (~(S(lookup::single_bit_set[m.to])));
		  }
		  else
		  {
			  pcBB[PieceType::wpawns] &= (~(N(lookup::single_bit_set[m.to])));
		  }
	  }
	  else if(m.isCapture)
	  {
		  MakeCaptureMove_NoUpdate(m.piece, m.captured, m.from, m.to);
	  }
	  else
	  {
		  MakeNormalMove_NoUpdate(m.piece, m.from, m.to);
		  if(m.piece == PieceType::wpawns && m.to != (m.from+8))
		  {
			  epSquare = Sq::ESq((unsigned)m.from+8);
		  }
		  else if(m.piece == PieceType::bpawns && m.to != (m.from-8))
		  {
			  epSquare = Sq::ESq((unsigned)m.from-8);
		  }
	  }

	  isWhitesTurn = !isWhitesTurn;

	  UpdateAll();

	  return IsLegal();
  }



  bool WhiteCanCastleKingSide() const { return castling[0]; }
  bool BlackCanCastleKingSide() const { return castling[2]; }
  bool WhiteCanCastleQueenSide() const { return castling[1]; }
  bool BlackCanCastleQueenSide() const { return castling[3]; }

  Sq::ESq EpSquare() const { return (epSquare); }
  void SetEpSquare(Sq::ESq sq)
  {
    epSquare = sq;
  }

  bool IsWhitesTurn() const { return isWhitesTurn; }

  unsigned HalfMoveClock() const { return halfMoveClock; }
  unsigned FullMoveCounter() const { return fullMoveCounter; }
  void IncrementHalfMoveClock() { ++halfMoveClock; }
  void IncrementFullMoveCounter() { ++fullMoveCounter; }

  ui64 PiecesAt( const PieceType::EPieceType p ) const { return pcBB[p]; }
  PieceType::EPieceType PieceAtSq( const Sq::ESq sq ) const
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
  void PutPieceAt( const PieceType::EPieceType p, const Sq::ESq sq )
  {
    PutPieceAt( p, SqFile(sq), SqRank(sq) );
    UpdateAll();
  }
  void ClearPieceAt( const PieceType::EPieceType p, const Sq::ESq sq )
  {
    ClearPieceAt( p, SqFile(sq), SqRank(sq) );
    UpdateAll();
  }
  void ClearPieceAt( const PieceType::EPieceType p, const int file, const int rank )
  {
    pcBB[p] &= (~(fileBB[file] & rankBB[rank]));
  }
  void PutPieceAt( const PieceType::EPieceType p, const int file, const int rank )
  {
    pcBB[p] |= ( fileBB[file] & rankBB[rank] );
  }
  void MakeNormalMove_NoUpdate( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeNormalMove(p,from,to) );

    ClearPieceAt(p, SqFile(from), SqRank(from) );
    PutPieceAt(p,  SqFile(to), SqRank(to) );
  }
  void MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    MakeNormalMove_NoUpdate(p,from,to);
    UpdateAll();
  }
  void MakeCaptureMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeCaptureMove(p,c,from,to) );

    const int toFile = SqFile(to);
    const int toRank = SqRank(to);

    ClearPieceAt(p, SqFile(from), SqRank(from));
    ClearPieceAt(c, toFile, toRank);
    PutPieceAt(p, toFile, toRank);
  }
  void MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
  {
	MakeCaptureMove_NoUpdate(p,c,from,to);
    UpdateAll();
  }
  void MakePromotionMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakePromotionMove(p,n,from,to) ) ;

    ClearPieceAt(p, SqFile(from), SqRank(from));
    PutPieceAt(n,  SqFile(to), SqRank(to) );
  }
  void MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    MakePromotionMove_NoUpdate(p, n, from, to);
    UpdateAll();
  }
  void MakeCapturePromotionMove_NoUpdate( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeCapturePromotionMove(p,c,n,from,to) );

    const int toFile = SqFile(to);
    const int toRank = SqRank(to);

    ClearPieceAt(p, SqFile(from), SqRank(from));
    ClearPieceAt(c, toFile, toRank);
    PutPieceAt(n, toFile, toRank);
    UpdateAll();
  }
  void MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    MakeCapturePromotionMove_NoUpdate(p,c,n,from,to);
    UpdateAll();
  }
  void UpdateWhite()
  {
    pcBB[PieceType::white] = Constants::clear;
    for( unsigned i=Ranges::wBegin; i<Ranges::wEnd; ++i )
    {
      assert( (pcBB[PieceType::white] & pcBB[i]) == Constants::clear );
      pcBB[PieceType::white] |= pcBB[i];
    }
  }
  void UpdateBlack()
  {
    pcBB[PieceType::black] = Constants::clear;
    for( unsigned i=Ranges::bBegin; i<Ranges::bEnd; ++i )
    {
      assert( (pcBB[PieceType::black] & pcBB[i]) == Constants::clear );
      pcBB[PieceType::black] |= pcBB[i];
    }
  }
  void UpdateOccupied()
  {
    assert( (pcBB[PieceType::black] & pcBB[PieceType::white]) == Constants::clear ); //No overlap
    DO_SAFE(occupiedBB = Constants::clear);
    occupiedBB = (pcBB[PieceType::black] | pcBB[PieceType::white]);
	pcBB[PieceType::all] = occupiedBB;
  }
  void UpdateEmpty()
  {
    DO_SAFE(emptyBB = Constants::clear);
    emptyBB = ~occupiedBB;
	pcBB[PieceType::none] = emptyBB;
  }
  void UpdateAll()
  {
    UpdateWhite();
    UpdateBlack();
    UpdateOccupied();
    UpdateEmpty();
  }

  void ASSERTS_MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
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
  void ASSERTS_MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    assert( IsSqOccupied(from) );
    assert( IsPawn(p) );
    assert( If_IsTrue( (PieceColor(p)==Color::white), (SqSetBit(to)&Constants::rank_8) ) );
    assert( If_IsTrue( (PieceColor(p)==Color::black), (SqSetBit(to)&Constants::rank_1) ) );
    assert( !IsPawn(n) );
    assert( !IsKing(n) );
    assert( PieceAtSq(from) == p );
  }
  void ASSERTS_MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
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
  void ASSERTS_MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    assert( IsSqEmpty(to) );
    assert( IsSqOccupied(from) );
    assert( PieceAtSq(from) == p );
  }
};


//Try using X_aligned_memcpy_sse2( &dst, &src, sizeof(Bitboard) );
inline void CopyBitboard( Bitboard& dst, const Bitboard& src ){ memcpy( &dst, &src, sizeof(Bitboard) ); }

//Try using X_aligned_memcpy_sse2( dst, src, sizeof(Bitboard) );
inline void CopyBitboard( Bitboard* dst, const Bitboard* src ){ memcpy( dst, src, sizeof(Bitboard) ); }

inline void ClearBitboard( Bitboard* b ){ memset( b, 0, sizeof(Bitboard) ); }

#endif



