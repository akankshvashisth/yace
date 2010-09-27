#ifndef __bitboard_fen_hpp
#define __bitboard_fen_hpp

//////////////////////////////////////////////////////////////////////////////////////
#include "fen_parser.hpp"
#include "bitboard.hpp"
#include <string>
#include <sstream>
#include "strings.hpp"

template <class T>
inline std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}


Bitboard BitboardFromFen( const std::string& fen )
{
  const FEN::BoardRepresentation& br = FEN::FenParser(fen);
  Bitboard bb;
  for(unsigned int i=0; i<64; ++i)
  {
    const char c = br.board[i];
    if(c != FEN::fen_clear)
    {
      PieceType::EPieceType p = PieceType::none;
      if(c == FEN::fen_black_king)
        p = PieceType::bking;
      else if(c == FEN::fen_black_knight)
        p = PieceType::bknights;
      else if(c == FEN::fen_black_bishop)
        p = PieceType::bbishops;
      else if(c == FEN::fen_black_rook)
        p = PieceType::brooks;
      else if(c == FEN::fen_black_queen)
        p = PieceType::bqueens;
      else if(c == FEN::fen_black_pawn)
        p = PieceType::bpawns;
      else if(c == FEN::fen_white_king)
        p = PieceType::wking;
      else if(c == FEN::fen_white_knight)
        p = PieceType::wknights;
      else if(c == FEN::fen_white_bishop)
        p = PieceType::wbishops;
      else if(c == FEN::fen_white_rook)
        p = PieceType::wrooks;
      else if(c == FEN::fen_white_queen)
        p = PieceType::wqueens;
      else if(c == FEN::fen_white_pawn)
        p = PieceType::wpawns;
      bb.PutPieceAt(p, Sq::ESq(i));
    }
  }

  for( unsigned i=0; i<4; ++i )
  {
    bb.castling[i] = br.castling[i];
  }
  
  bb.halfMoveClock = br.halfMoveClock;
  bb.fullMoveCounter = br.fullMoveCounter;
  bb.isWhitesTurn = br.isWhitesTurn;
  bb.epSquare = (br.epSquare < 0) ? Sq::none : (Sq::ESq)(br.epSquare);  

  return bb;
}



std::string FenFromBitboard( Bitboard b )
{
  const std::string sempty("");
  const std::string space(" ");
 
  std::string s[FEN::FEN_NONE];

  s[FEN::SideToMove] = b.IsWhitesTurn() ? std::string("w") : std::string("b");
  
  s[FEN::EpTargetSquare] = b.EpSquare() != Sq::none ? Sq::SSq[b.EpSquare()] : std::string("-");

  s[FEN::CastlingAbility].clear();
  s[FEN::CastlingAbility] += b.WhiteCanCastleKingSide()      ? std::string("K") : sempty;
  s[FEN::CastlingAbility] += b.WhiteCanCastleQueenSide()     ? std::string("Q") : sempty;
  s[FEN::CastlingAbility] += b.BlackCanCastleKingSide()      ? std::string("k") : sempty;
  s[FEN::CastlingAbility] += b.BlackCanCastleQueenSide()     ? std::string("q") : sempty;
  s[FEN::CastlingAbility] += s[FEN::CastlingAbility].empty() ? std::string("-") : sempty;

  s[FEN::FullMoveCounter] = to_string(b.FullMoveCounter());

  s[FEN::HalfMoveClock] = to_string(b.HalfMoveClock());

  s[FEN::PiecePlacement] = sempty;
  unsigned n=0;
  for(int i=7; i>=0; --i)
  {
    for(int j=0; j<8; ++j)
    {
      Sq::ESq sq = SquareFromFileRank(Files::EFiles(j), Ranks::ERanks(i));
      PieceType::EPieceType p = b.PieceAtSq(sq);
      std::string st = sempty;
           if( p == PieceType::wking)    { st = std::string("K"); }
      else if( p == PieceType::bking)    { st = std::string("k"); }
      else if( p == PieceType::wqueens)  { st = std::string("Q"); }
      else if( p == PieceType::bqueens)  { st = std::string("q"); }
      else if( p == PieceType::wknights) { st = std::string("N"); }
      else if( p == PieceType::bknights) { st = std::string("n"); }
      else if( p == PieceType::wrooks)   { st = std::string("R"); }
      else if( p == PieceType::brooks)   { st = std::string("r"); }
      else if( p == PieceType::wbishops) { st = std::string("B"); }
      else if( p == PieceType::bbishops) { st = std::string("b"); }
      else if( p == PieceType::wpawns)   { st = std::string("P"); }
      else if( p == PieceType::bpawns)   { st = std::string("p"); }

      if(!st.empty())
      {
        if( n != 0 )
        {
          s[FEN::PiecePlacement] += to_string(n);
          n = 0;
        }
        s[FEN::PiecePlacement] += st; 
      }
      else
      {
        ++n;
      }
    }
    if(n!=0)
    {
      s[FEN::PiecePlacement] += to_string(n);
      n = 0;
    }
    if(i!=0)
    {
      s[FEN::PiecePlacement] += "/";
    }
  }

  std::string toReturn = s[FEN::PiecePlacement]  + space + 
                         s[FEN::SideToMove]      + space + 
                         s[FEN::CastlingAbility] + space +
                         s[FEN::EpTargetSquare]  + space +
                         s[FEN::HalfMoveClock]   + space +
                         s[FEN::FullMoveCounter];

  return toReturn;
}


//////////////////////////////////////////////////////////////////////////////////////
#endif