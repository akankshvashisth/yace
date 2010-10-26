#ifndef __evaluation_constants_hpp
#define __evaluation_constants_hpp

#include "enums.hpp"
#include "constants.hpp"

namespace PieceValues
{
  static const int INFINITY = 1000000;
  static const int size = 15;

  namespace MiddleGame
  {
    static const int pawn    = 100;
    static const int knight  = 300;
    static const int bishop  = 300;
    static const int rook    = 500;
    static const int queen   = 900;
  }

  namespace Endgame
  {
    static const int pawn    = 150;
    static const int knight  = 250;
    static const int bishop  = 350;
    static const int rook    = 600;
    static const int queen   = 1000;
  }

  static int MiddleGame_Table[size];
  static int EndGame_Table[size];
}

namespace SquareTables //From white side!
{
  static const int size = 64;
  static int MiddleGameTable[PieceType::none][size];
  static int EndGameTable[PieceType::none][size];

  static int pawn_middlegame[] = 
  {
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
  };

  static int knight_middlegame[] = 
  {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
  };

  static int bishop_middlegame[] = 
  {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
  };

  static int rook_middlegame[] = 
  {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     0,  0,  0,  5,  5,  0,  0,  0
  };

  static int queen_middlegame[] =
  {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
  };

    static int pawn_endgame[] = 
  {
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
  };

  static int knight_endgame[] = 
  {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
  };

  static int bishop_endgame[] = 
  {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
  };

  static int rook_endgame[] = 
  {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     0,  0,  0,  5,  5,  0,  0,  0
  };

  static int queen_endgame[] =
  {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
  };

  static int king_middle_game[] = 
  {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
  };

  static int king_end_game[] = 
  {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
  };
}

namespace MobilityTables
{
  static const int size = 15;

  static int bishop[]   = { -6, -2, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8 };
  static int rook[]     = { -4, -2, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4 };
  static int knight[]   = { -6, -4, 0, 2, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 };
  static int queen[]    = { -4, -2, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4 };
  static int king[]     = {  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  static int pawn[]     = {  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  static int Table[PieceType::none][size];
}

namespace PawnBasedValueAdjust
{
  static const int size = 9;

  static int bishop[] = {  12,   8,   4,  0, -4, -8, -12, -16, -20 }; //reverse of knight
  static int knight[] = { -20, -16, -12, -8, -4,  0,   4,   8,  12 };
  static int rook[]   = {  15,  12,   9,  6,  3,  0,  -3,  -6,  -9 };
  static int queen[]  = {  0,    0,   0,  0,  0,  0,   0,   0,   0 };
  static int king[]   = {  0,    0,   0,  0,  0,  0,   0,   0,   0 };
  static int pawn[]   = {  0,    0,   0,  0,  0,  0,   0,   0,   0 };

  static int Table[PieceType::none][size];
}

static void init_eval_constants()
{
  for(unsigned i=0; i<SquareTables::size; ++i)
  {
    SquareTables::MiddleGameTable[PieceType::wpawns][i]   = SquareTables::pawn_middlegame[i];
    SquareTables::MiddleGameTable[PieceType::wknights][i] = SquareTables::knight_middlegame[i];
    SquareTables::MiddleGameTable[PieceType::wbishops][i] = SquareTables::bishop_middlegame[i];
    SquareTables::MiddleGameTable[PieceType::wrooks][i]   = SquareTables::rook_middlegame[i];
    SquareTables::MiddleGameTable[PieceType::wqueens][i]  = SquareTables::queen_middlegame[i];
    SquareTables::MiddleGameTable[PieceType::wking][i]    = SquareTables::king_middle_game[i];

    SquareTables::MiddleGameTable[PieceType::bpawns][i]   = SquareTables::pawn_middlegame[SquareTables::size-1-i];
    SquareTables::MiddleGameTable[PieceType::bknights][i] = SquareTables::knight_middlegame[SquareTables::size-1-i];
    SquareTables::MiddleGameTable[PieceType::bbishops][i] = SquareTables::bishop_middlegame[SquareTables::size-1-i];
    SquareTables::MiddleGameTable[PieceType::brooks][i]   = SquareTables::rook_middlegame[SquareTables::size-1-i];
    SquareTables::MiddleGameTable[PieceType::bqueens][i]  = SquareTables::queen_middlegame[SquareTables::size-1-i];
    SquareTables::MiddleGameTable[PieceType::bking][i]    = SquareTables::king_middle_game[SquareTables::size-1-i];

    SquareTables::EndGameTable[PieceType::wpawns][i]   = SquareTables::pawn_endgame[i];
    SquareTables::EndGameTable[PieceType::wknights][i] = SquareTables::knight_endgame[i];
    SquareTables::EndGameTable[PieceType::wbishops][i] = SquareTables::bishop_endgame[i];
    SquareTables::EndGameTable[PieceType::wrooks][i]   = SquareTables::rook_endgame[i];
    SquareTables::EndGameTable[PieceType::wqueens][i]  = SquareTables::queen_endgame[i];
    SquareTables::EndGameTable[PieceType::wking][i]    = SquareTables::king_end_game[i];

    SquareTables::EndGameTable[PieceType::bpawns][i]   = SquareTables::pawn_endgame[SquareTables::size-1-i];
    SquareTables::EndGameTable[PieceType::bknights][i] = SquareTables::knight_endgame[SquareTables::size-1-i];
    SquareTables::EndGameTable[PieceType::bbishops][i] = SquareTables::bishop_endgame[SquareTables::size-1-i];
    SquareTables::EndGameTable[PieceType::brooks][i]   = SquareTables::rook_endgame[SquareTables::size-1-i];
    SquareTables::EndGameTable[PieceType::bqueens][i]  = SquareTables::queen_endgame[SquareTables::size-1-i];
    SquareTables::EndGameTable[PieceType::bking][i]    = SquareTables::king_end_game[SquareTables::size-1-i];

  }

  for(unsigned i=0; i<MobilityTables::size; ++i)
  {
    MobilityTables::Table[PieceType::wpawns][i]   = MobilityTables::pawn[i];
    MobilityTables::Table[PieceType::wknights][i] = MobilityTables::knight[i];
    MobilityTables::Table[PieceType::wbishops][i] = MobilityTables::bishop[i];
    MobilityTables::Table[PieceType::wrooks][i]   = MobilityTables::rook[i];
    MobilityTables::Table[PieceType::wqueens][i]  = MobilityTables::queen[i];
    MobilityTables::Table[PieceType::wking][i]    = MobilityTables::king[i];
                                                                    
    MobilityTables::Table[PieceType::bpawns][i]   = MobilityTables::pawn[i];
    MobilityTables::Table[PieceType::bknights][i] = MobilityTables::knight[i];
    MobilityTables::Table[PieceType::bbishops][i] = MobilityTables::bishop[i];
    MobilityTables::Table[PieceType::brooks][i]   = MobilityTables::rook[i];
    MobilityTables::Table[PieceType::bqueens][i]  = MobilityTables::queen[i];
    MobilityTables::Table[PieceType::bking][i]    = MobilityTables::king[i];
  }

  for(unsigned i=0; i<PawnBasedValueAdjust::size; ++i)
  {
    PawnBasedValueAdjust::Table[PieceType::wpawns][i]   = PawnBasedValueAdjust::pawn[i];
    PawnBasedValueAdjust::Table[PieceType::wknights][i] = PawnBasedValueAdjust::knight[i];
    PawnBasedValueAdjust::Table[PieceType::wbishops][i] = PawnBasedValueAdjust::bishop[i];
    PawnBasedValueAdjust::Table[PieceType::wrooks][i]   = PawnBasedValueAdjust::rook[i];
    PawnBasedValueAdjust::Table[PieceType::wqueens][i]  = PawnBasedValueAdjust::queen[i];
    PawnBasedValueAdjust::Table[PieceType::wking][i]    = PawnBasedValueAdjust::king[i];
                                                                    
    PawnBasedValueAdjust::Table[PieceType::bpawns][i]   = PawnBasedValueAdjust::pawn[i];
    PawnBasedValueAdjust::Table[PieceType::bknights][i] = PawnBasedValueAdjust::knight[i];
    PawnBasedValueAdjust::Table[PieceType::bbishops][i] = PawnBasedValueAdjust::bishop[i];
    PawnBasedValueAdjust::Table[PieceType::brooks][i]   = PawnBasedValueAdjust::rook[i];
    PawnBasedValueAdjust::Table[PieceType::bqueens][i]  = PawnBasedValueAdjust::queen[i];
    PawnBasedValueAdjust::Table[PieceType::bking][i]    = PawnBasedValueAdjust::king[i];
  }

  for(unsigned i=0; i<PieceValues::size; ++i)
  {
    PieceValues::EndGame_Table[PieceType::wpawns]   = PieceValues::Endgame::pawn;
    PieceValues::EndGame_Table[PieceType::wknights] = PieceValues::Endgame::knight;
    PieceValues::EndGame_Table[PieceType::wbishops] = PieceValues::Endgame::bishop;
    PieceValues::EndGame_Table[PieceType::wrooks]   = PieceValues::Endgame::rook;
    PieceValues::EndGame_Table[PieceType::wqueens]  = PieceValues::Endgame::queen;
    PieceValues::EndGame_Table[PieceType::wking]    = PieceValues::INFINITY;

    PieceValues::EndGame_Table[PieceType::bpawns]   = PieceValues::Endgame::pawn;
    PieceValues::EndGame_Table[PieceType::bknights] = PieceValues::Endgame::knight;
    PieceValues::EndGame_Table[PieceType::bbishops] = PieceValues::Endgame::bishop;
    PieceValues::EndGame_Table[PieceType::brooks]   = PieceValues::Endgame::rook;
    PieceValues::EndGame_Table[PieceType::bqueens]  = PieceValues::Endgame::queen;
    PieceValues::EndGame_Table[PieceType::bking]    = PieceValues::INFINITY;

    PieceValues::MiddleGame_Table[PieceType::wpawns]   = PieceValues::MiddleGame::pawn;
    PieceValues::MiddleGame_Table[PieceType::wknights] = PieceValues::MiddleGame::knight;
    PieceValues::MiddleGame_Table[PieceType::wbishops] = PieceValues::MiddleGame::bishop;
    PieceValues::MiddleGame_Table[PieceType::wrooks]   = PieceValues::MiddleGame::rook;
    PieceValues::MiddleGame_Table[PieceType::wqueens]  = PieceValues::MiddleGame::queen;
    PieceValues::MiddleGame_Table[PieceType::wking]    = PieceValues::INFINITY;

    PieceValues::MiddleGame_Table[PieceType::bpawns]   = PieceValues::MiddleGame::pawn;
    PieceValues::MiddleGame_Table[PieceType::bknights] = PieceValues::MiddleGame::knight;
    PieceValues::MiddleGame_Table[PieceType::bbishops] = PieceValues::MiddleGame::bishop;
    PieceValues::MiddleGame_Table[PieceType::brooks]   = PieceValues::MiddleGame::rook;
    PieceValues::MiddleGame_Table[PieceType::bqueens]  = PieceValues::MiddleGame::queen;
    PieceValues::MiddleGame_Table[PieceType::bking]    = PieceValues::INFINITY;
  }
}

//namespace Contempts
//{
//  static int black = 0;
//  static int white = 0;
//}

#endif