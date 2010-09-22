#ifndef __show_hpp
#define __show_hpp

#include <iostream>
#include "bitboard.hpp"
#include "typedefs.hpp"
#include "bitfunctions.hpp"
#include "strings.hpp"

namespace ShowTypes
{
  struct Console{};
  struct File{};
}

template<typename T>
struct Show;

template<>
struct Show<ShowTypes::Console>
{
  template<typename T>
  static void Op( const T& t )
  {
    std::cout << t << std::endl;
  }

  template<>
  static void Op<Bitboard>( const Bitboard& b )
  {
    for( int i=7; i>=0; --i )
    {
      for( int j=0; j<8; ++j )
      {
        const Sq::ESq sq = (Sq::ESq)(i*8+j);
        const PieceType::EPieceType p = b.PieceAtSq( sq );
        std::cout << PieceType::SPieceType_short[p];
      }
      std::cout << std::endl;
    }
  }

  template<>
  static void Op<ui64>( const ui64& b )
  {
    for( int i=7; i>=0; --i )
    {
      for( int j=0; j<8; ++j )
      {
        std::cout << IsBitSet(b, i*8+j) ? 1 : 0 ;
        //std::cout << Sq::SSq[i*8+j] << " ";
      }
      std::cout << std::endl;
    }
  }
};

#endif 