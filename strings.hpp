#ifndef __strings_hpp
#define __strings_hpp

#include <string>

namespace PieceType
{
  static const std::string SPieceType[] =
  {
    "white",
    "wpawns",
    "wknights",
    "wbishops",
    "wrooks",
    "wqueens",
    "wking",
    "black",
    "bpawns",
    "bknights",
    "bbishops",
    "brooks",
    "bqueens",
    "bking",
    "all",
    "none"
  };

  static const std::string SPieceType_short[] =
  {
    "white",
    "P",
    "N",
    "B",
    "R",
    "Q",
    "K",
    "black",
    "p",
    "n",
    "b",
    "r",
    "q",
    "k",
    "all",
    "."
  };
}

namespace Color
{
  static const std::string SColor[] =
  {
    "white",
    "black",
    "none"
  };
}

namespace Sq
{
  static const std::string SSq[] = 
  {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "none"
  };
}


#endif