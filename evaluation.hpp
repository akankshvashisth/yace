#ifndef __evaluation_hpp
#define __evaluation_hpp

#include "typedefs.hpp"
#include "evaluation_constants.hpp"
#include "bitboard.hpp"

int squareEval(BB p, int p_c, PieceType::EPieceType piece_type)
{
  int p_s = 0;
  for(int i=0; i<p_c; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(p);
		p &= (lookup::not_single_bit_set[sq]);
    p_s += SquareTables::MiddleGameTable[piece_type][sq];
  }
  return p_s;
}

//material + location + evaluation for middle game only. 
//mobility also can be added
//can be done in move_gen also, as all these things are done there too. and mobility would be much simpler.
int evaluate( Bitboard& b ) 
{
  BB wP = b.PiecesAt(PieceType::wpawns  );
  BB wN = b.PiecesAt(PieceType::wknights);
  BB wB = b.PiecesAt(PieceType::wbishops);
  BB wR = b.PiecesAt(PieceType::wrooks  );
  BB wQ = b.PiecesAt(PieceType::wqueens );
  BB wK = b.PiecesAt(PieceType::wking   );

  BB bP = b.PiecesAt(PieceType::bpawns);
  BB bN = b.PiecesAt(PieceType::bknights);
  BB bB = b.PiecesAt(PieceType::bbishops);
  BB bR = b.PiecesAt(PieceType::brooks);
  BB bQ = b.PiecesAt(PieceType::bqueens);
  BB bK = b.PiecesAt(PieceType::bking);

  int wP_c = PopulationCount(wP);
  int wN_c = PopulationCount(wN);
  int wB_c = PopulationCount(wB);
  int wR_c = PopulationCount(wR);
  int wQ_c = PopulationCount(wQ);
  int wK_c = PopulationCount(wK);

  int bP_c = PopulationCount(bP);
  int bN_c = PopulationCount(bN);
  int bB_c = PopulationCount(bB);
  int bR_c = PopulationCount(bR);
  int bQ_c = PopulationCount(bQ);
  int bK_c = PopulationCount(bK);

  int wP_m = wP_c * PieceValues::MiddleGame_Table[PieceType::wpawns  ];
  int wN_m = wN_c * PieceValues::MiddleGame_Table[PieceType::wknights];
  int wB_m = wB_c * PieceValues::MiddleGame_Table[PieceType::wbishops];
  int wR_m = wR_c * PieceValues::MiddleGame_Table[PieceType::wrooks  ];
  int wQ_m = wQ_c * PieceValues::MiddleGame_Table[PieceType::wqueens ];
  int wK_m = wK_c * PieceValues::MiddleGame_Table[PieceType::wking   ];

  int bP_m = bP_c * PieceValues::MiddleGame_Table[PieceType::bpawns  ];
  int bN_m = bN_c * PieceValues::MiddleGame_Table[PieceType::bknights];
  int bB_m = bB_c * PieceValues::MiddleGame_Table[PieceType::bbishops];
  int bR_m = bR_c * PieceValues::MiddleGame_Table[PieceType::brooks  ];
  int bQ_m = bQ_c * PieceValues::MiddleGame_Table[PieceType::bqueens ];
  int bK_m = bK_c * PieceValues::MiddleGame_Table[PieceType::bking   ];
  
  int material_score = (wP_m - bP_m) +
                       (wN_m - bN_m) +
                       (wB_m - bB_m) +
                       (wR_m - bR_m) +
                       (wQ_m - bQ_m) +
                       (wK_m - bK_m);

  int wP_s = squareEval(wP, wP_c, PieceType::wpawns  );
  int wN_s = squareEval(wN, wN_c, PieceType::wknights);
  int wB_s = squareEval(wB, wB_c, PieceType::wbishops);
  int wR_s = squareEval(wR, wR_c, PieceType::wrooks  );
  int wQ_s = squareEval(wQ, wQ_c, PieceType::wqueens );
  int wK_s = squareEval(wK, wK_c, PieceType::wking   );

  int bP_s = squareEval(bP, bP_c, PieceType::bpawns  );
  int bN_s = squareEval(bN, bN_c, PieceType::bknights);
  int bB_s = squareEval(bB, bB_c, PieceType::bbishops);
  int bR_s = squareEval(bR, bR_c, PieceType::brooks  );
  int bQ_s = squareEval(bQ, bQ_c, PieceType::bqueens );
  int bK_s = squareEval(bK, bK_c, PieceType::bking   );

  int square_score = (wP_s - bP_s) +
                     (wN_s - bN_s) +
                     (wB_s - bB_s) +
                     (wR_s - bR_s) +
                     (wQ_s - bQ_s) +
                     (wK_s - bK_s);

  int multiplier = b.IsWhitesTurn() ? 1 : -1;

  int evaluation = ( material_score + square_score ) * multiplier;
  return evaluation;
}
 


#endif

