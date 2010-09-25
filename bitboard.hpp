
#ifndef __bitboard_hpp
#define __bitboard_hpp

#include "constants.hpp"
#include "defines.hpp"
#include "enums.hpp"
#include "typedefs.hpp"
#include "chessutils.hpp"
#include "utility.hpp"
#include <cstring>


struct Bitboard;

inline void CopyBitboard( Bitboard& dst, const Bitboard& src );
inline void CopyBitboard( Bitboard* dst, const Bitboard* src );
inline void ClearBitboard( Bitboard* b );

struct Bitboard
{
public:
  BB pcBB[14];
  BB emptyBB;
  BB occupiedBB;
public:
  Bitboard()
  {
    ClearBitboard(this);
  }
  Bitboard( const Bitboard& other )
  {
    CopyBitboard( this, &other );
  }
  bool IsSqEmpty( const Sq::ESq sq )
  {
    return IS_TRUE(SqSetBit(sq) & emptyBB);
  }
  bool IsSqOccupied( const Sq::ESq sq )
  {
    return IS_TRUE(SqSetBit(sq) & occupiedBB);
  }
  ui64 PiecesAt( const PieceType::EPieceType p ) const
  {
    return pcBB[p];
  }
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
  void MakeNormalMove( const PieceType::EPieceType p, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeNormalMove(p,from,to) );

    ClearPieceAt(p, SqFile(from), SqRank(from) );
    PutPieceAt(p,  SqFile(to), SqRank(to) );
    UpdateAll();
  }
  void MakeCaptureMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeCaptureMove(p,c,from,to) );

    const int toFile = SqFile(to);
    const int toRank = SqRank(to);

    ClearPieceAt(p, SqFile(from), SqRank(from));
    ClearPieceAt(c, toFile, toRank);
    PutPieceAt(p, toFile, toRank);
    UpdateAll();
  }
  void MakePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakePromotionMove(p,n,from,to) ) ;

    ClearPieceAt(p, SqFile(from), SqRank(from));
    PutPieceAt(n,  SqFile(to), SqRank(to) );
    UpdateAll();
  }
  void MakeCapturePromotionMove( const PieceType::EPieceType p, const PieceType::EPieceType c, const PieceType::EPieceType n, const Sq::ESq from, const Sq::ESq to )
  {
    DO_SAFE( ASSERTS_MakeCapturePromotionMove(p,c,n,from,to) );

    const int toFile = SqFile(to);
    const int toRank = SqRank(to);

    ClearPieceAt(p, SqFile(from), SqRank(from));
    ClearPieceAt(c, toFile, toRank);
    PutPieceAt(n, toFile, toRank);
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
  }
  void UpdateEmpty()
  {
    DO_SAFE(emptyBB = Constants::clear);
    emptyBB = ~occupiedBB;
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


inline void CopyBitboard( Bitboard& dst, const Bitboard& src )
{
  //X_aligned_memcpy_sse2( &dst, &src, sizeof(Bitboard) );
  memcpy( &dst, &src, sizeof(Bitboard) );
}

inline void CopyBitboard( Bitboard* dst, const Bitboard* src )
{
  //X_aligned_memcpy_sse2( dst, src, sizeof(Bitboard) );
  memcpy( dst, src, sizeof(Bitboard) );
}

inline void ClearBitboard( Bitboard* b )
{
  memset( b, 0, sizeof(Bitboard) );
}



#endif



