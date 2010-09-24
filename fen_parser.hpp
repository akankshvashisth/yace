#ifndef __fen_parser_hpp
#define __fen_parser_hpp

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cstdlib>

namespace FEN
{
class Appender : public std::vector<std::string> {
public:
  void operator() (const char* s, unsigned length) { 
    this->push_back(std::string(s,length));
  }
};

template<typename Operator>
void tokenize(Operator& op, const char* input, const char* delimiters) {
  const char* s = input;
  const char* e = s;
  while (*e != 0) {
    e = s;
    while (*e != 0 && strchr(delimiters, *e) == 0) ++e;
    if (e - s > 0) {
      op(s, e - s);
    }
    s = e + 1;
  }
}


char fen_WhitePieceName[6] = {'P', 'N', 'B', 'R', 'Q', 'K'};
char fen_BlackPieceName[6] = {'p', 'n', 'b', 'r', 'q', 'k'};
char fen_File[8] = {'a','b','c','d','e','f','g','h'};
char fen_Rank[8] = {'1','2','3','4','5','6','7','8'};
enum fen_Piece{ fen_pawn, fen_knight, fen_bishop, fen_rook, fen_queen, fen_king };

struct BoardRepresentation
{
  static const unsigned int SIZE = 64;
  BoardRepresentation()
  {
    for(unsigned i=0; i<SIZE; ++i)
      board[i] = 'X';
    isWhitesTurn = true;
    for(unsigned i=0; i<4; ++i)
      castling[i] = false;
    hasEpSquare = false;
    epSquare = -1;
    halfMoveClock = 0;
  }

  char board[SIZE];
  bool isWhitesTurn;
  bool castling[4]; // WK, WQ, BK, BQ
  bool hasEpSquare;
  int epSquare;
  unsigned int halfMoveClock;
  unsigned int fullMoveCounter;
};

enum FenTokens{ PiecePlacement, SideToMove, CastlingAbility, EpTargetSquare, HalfMoveClock, FullMoveCounter } ;

BoardRepresentation FenParser( const std::string& fen )
{
  std::cout << "-----------------------" << std::endl;
  BoardRepresentation br;
  std::istringstream iss(fen);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter<std::vector<std::string> >(tokens));

  Appender v;
  tokenize(v, tokens[PiecePlacement].c_str(), "/");

  int n=0;
  for(int i=7; i>=0; --i)
  {
    for(int j=0; j<v[i].size(); ++j)
    {
      if     (v[i][j] == 'k')
      {
        br.board[n] = fen_BlackPieceName[fen_king];
        ++n;
      }
      else if(v[i][j] == 'K')
      {
        br.board[n] = fen_WhitePieceName[fen_king];
        ++n;
      }
      else if(v[i][j] == 'q')
      {
        br.board[n] = fen_BlackPieceName[fen_queen];
        ++n;
      }
      else if(v[i][j] == 'Q')
      {
        br.board[n] = fen_WhitePieceName[fen_queen];
        ++n;
      }
      else if(v[i][j] == 'r')
      {
        br.board[n] = fen_BlackPieceName[fen_rook];
        ++n;
      }
      else if(v[i][j] == 'R')
      {
        br.board[n] = fen_WhitePieceName[fen_rook];
        ++n;
      }
      else if(v[i][j] == 'b')
      {
        br.board[n] = fen_BlackPieceName[fen_bishop];
        ++n;
      }
      else if(v[i][j] == 'B')
      {
        br.board[n] = fen_WhitePieceName[fen_bishop];
        ++n;
      }
      else if(v[i][j] == 'n')
      {
        br.board[n] = fen_BlackPieceName[fen_knight];
        ++n;
      }
      else if(v[i][j] == 'N')
      {
        br.board[n] = fen_WhitePieceName[fen_knight];
        ++n;
      }
      else if(v[i][j] == 'p')
      {
        br.board[n] = fen_BlackPieceName[fen_pawn];
        ++n;
      }
      else if(v[i][j] == 'P')
      {
        br.board[n] = fen_WhitePieceName[fen_pawn];
        ++n;
      }
      else if(v[i][j] == '1')
      {
        n += 1;
      }
      else if(v[i][j] == '2')
      {
        n += 2;
      }
      else if(v[i][j] == '3')
      {
        n += 3;
      }
      else if(v[i][j] == '4')
      {
        n += 4;
      }
      else if(v[i][j] == '5')
      {
        n += 5;
      }
      else if(v[i][j] == '6')
      {
        n += 6;
      }
      else if(v[i][j] == '7')
      {
        n += 7;
      }
      else if(v[i][j] == '8')
      {
        n += 8;
      }
    }
  }

  if( tokens[EpTargetSquare][0] != '-' && tokens[EpTargetSquare].size() == 2)
  {
    char file = tokens[EpTargetSquare][0];
    char rank = tokens[EpTargetSquare][1];

    int f = 0;
    int r = 0;
    for(; f<8; ++f)
      if(fen_File[f] == file)
        break;
    for(; r<8; ++r)
      if(fen_File[r] == rank)
        break;

    br.hasEpSquare = true;
    br.epSquare = (r*8)+f;
  }
  else
    br.hasEpSquare = false;

  if(!tokens[CastlingAbility].empty() && tokens[CastlingAbility][0] != '-')
  {
    for(unsigned i=0; i<tokens[CastlingAbility].size(); ++i)
    {
      const char c = tokens[CastlingAbility][i];
      if( c == 'K' )
        br.castling[0] = true;
      else if( c == 'Q' )
        br.castling[1] = true;
      else if( c == 'k' )
        br.castling[2] = true;
      else if( c == 'q' )
        br.castling[3] = true;
    }
  }
  else
  {
    br.castling[0] = false;
    br.castling[1] = false;
    br.castling[2] = false;
    br.castling[3] = false;
  }

  br.isWhitesTurn = ((tokens[SideToMove] == std::string("w"))||(tokens[SideToMove] == std::string("W")));
  br.halfMoveClock = atoi( tokens[HalfMoveClock].c_str() );
  br.fullMoveCounter = atoi( tokens[FullMoveCounter].c_str() );


  for(int i=7; i>=0; --i)
  {
    for(unsigned int j=0; j<8; ++j)
    {
      std::cout << br.board[(i*8)+j];
    }
    std::cout << std::endl;
  }
  std::cout << br.halfMoveClock << std::endl;
  std::cout << br.fullMoveCounter << std::endl;
  std::cout << br.isWhitesTurn << std::endl;
  std::cout << br.castling[0] << ", " << br.castling[1] << ", " << br.castling[2] << ", " << br.castling[3] << std::endl;
  std::cout << br.hasEpSquare << " -> " << br.epSquare << std::endl;

  return br;
}
}


#endif