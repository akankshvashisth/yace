#ifndef __enums_hpp
#define __enums_hpp

namespace PieceType
{
  enum EPieceType
  {
    white,
    wpawns,
    wknights,
    wbishops,
    wrooks,
    wqueens,
    wking,
    black,
    bpawns,
    bknights,
    bbishops,
    brooks,
    bqueens,
    bking,
    all, 
    none
  };
}


namespace Color { enum EColor { white, black, none }; }


namespace Sq
{
  enum ESq
  {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    none
  };
}

namespace Ranges
{
  enum ERanges
  {
    wBegin = PieceType::wpawns,
    wEnd = PieceType::black,
    bBegin = PieceType::bpawns,
    bEnd = PieceType::all
  };
}

namespace Files { enum EFiles { file_1, file_2, file_3, file_4, file_5, file_6, file_7, file_8, none }; }
namespace Ranks { enum ERanks { rank_1, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, none }; }

namespace ValueType
{
  enum EValueType
  {
    value_type_upper = 1,  // Upper bound
    value_type_lower = 2,  // Lower bound
    value_type_exact = value_type_upper | value_type_lower    
  };
}

namespace Values
{
  enum EValues
  {
    value_draw          = 0,
    value_known_win     = 15000,
    value_mate          = 30000,
    value_infinite      = 30001,
    value_none          = 30002,
    value_ensure_signed = -1
  };
}



#endif