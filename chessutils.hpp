#ifndef __chessutils_hpp
#define __chessutils_hpp

#include "enums.hpp"
#include "typedefs.hpp"

inline Color::EColor PieceColor( const PieceType::EPieceType p )
{
  return (p < PieceType::black) ? Color::white : Color::black;
}
inline bool IsKing( const PieceType::EPieceType p )
{
  return ((p==PieceType::wking) || (p==PieceType::bking));
}
inline bool IsPawn( const PieceType::EPieceType p )
{
  return ((p==PieceType::wpawns) || (p==PieceType::bpawns));
}

inline int SqFile( const Sq::ESq sq ){ return sq&7; }
inline int SqRank( const Sq::ESq sq ){ return sq>>3; }
inline ui64 SqSetBit( const Sq::ESq sq ){ return ui64(1)<<sq; }


#endif
