#ifndef __show_hpp
#define __show_hpp

#include <iostream>
#include "bitboard.hpp"
#include "typedefs.hpp"
#include "bitfunctions.hpp"
#include "strings.hpp"
#include "fen_parser.hpp"

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
  static void Op( const T& t );
};

  template<typename T>
  void  Show<ShowTypes::Console>::Op( const T& t )
  {
    std::cout << t << std::endl;
  }

  template<>
  void  Show<ShowTypes::Console>::Op<Bitboard>( const Bitboard& b )
  {
    std::cout << "--------" << std::endl;
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
    std::cout << "--------" << std::endl;
    std::cout << "Full Move Counter : " << b.FullMoveCounter() << std::endl;
    std::cout << "Half Move Clock   : " << b.HalfMoveClock() << std::endl;
    std::cout << "Turn              : " << (b.IsWhitesTurn() ? "White" : "Black") << std::endl;
    std::cout << "Ep Square         : " << Sq::SSq[b.EpSquare()] << std::endl;
    if( b.WhiteCanCastleKingSide()  || 
        b.BlackCanCastleKingSide()  ||
        b.WhiteCanCastleQueenSide() ||
        b.BlackCanCastleQueenSide() )
    {
      std::cout << "Castling Rights   : ";
      if(b.WhiteCanCastleKingSide() ) std::cout << "K ";
      if(b.WhiteCanCastleQueenSide()) std::cout << "Q ";
      if(b.BlackCanCastleKingSide() ) std::cout << "k ";
      if(b.BlackCanCastleQueenSide()) std::cout << "q ";
      std::cout << std::endl;
    }
  }

  template<>
  void  Show<ShowTypes::Console>::Op<FEN::BoardRepresentation>( const FEN::BoardRepresentation& br )
  {
    std::cout << "**************FEN****************" << std::endl;
		std::cout << "-------------BOARD---------------" << std::endl;
		for(int i=7; i>=0; --i)
		{
			for(unsigned int j=0; j<8; ++j)
			{
				std::cout << br.board[(i*8)+j];
			}
			std::cout << std::endl;
		}
		std::cout << "---------HALF MOVE CLOCK---------" << std::endl;
		std::cout << br.halfMoveClock << std::endl;
		std::cout << "--------FULL MOVE COUNTER--------" << std::endl;
		std::cout << br.fullMoveCounter << std::endl;
		std::cout << "---------IS WHITES TURN----------" << std::endl;
		std::cout <<( br.isWhitesTurn ? "yes" : "no" )<< std::endl;
		std::cout << "---------CASTLING RIGHTS---------" << std::endl;
		std::cout << (br.castling[0] ? 'K' : '-' )<< ", "
			        << (br.castling[1] ? 'Q' : '-' )<< ", "
				      << (br.castling[2] ? 'k' : '-' )<< ", "
				      << (br.castling[3] ? 'q' : '-' )<< std::endl;
		std::cout << "----------HAS EP SQUARE----------" << std::endl;
		std::cout << (br.hasEpSquare ? "yes" : "no") << std::endl;
		if(br.hasEpSquare)
		{
			std::cout << "------------EP SQUARE------------" << std::endl;
      std::cout <<  Sq::SSq[br.epSquare] << std::endl;
		}
		std::cout << "*********************************" << std::endl;
  }

  template<>
  void  Show<ShowTypes::Console>::Op<ui64>( const ui64& b )
  {
    for( int i=7; i>=0; --i )
    {
      for( int j=0; j<8; ++j )
      {
        std::cout << (IsBitSet(b, i*8+j) ? 1 : 0) ;
        //std::cout << Sq::SSq[i*8+j] << " ";
      }
      std::cout << std::endl;
    }
  }

#endif
