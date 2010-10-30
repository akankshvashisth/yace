#include "bitboard.hpp"
#include "show.hpp"
#include "bbfunctions.hpp"
#include "fen_parser.hpp"
#include "random.hpp"
#include "bitboard_fen.hpp"
#include "lookup.hpp"
#include "piece_attacks.hpp"
#include "move_gen.hpp"
#include "hiResTime.hpp"
#include "init.hpp"
#include "bitboard_zobrist.hpp"
#include "tt.hpp"
#include "perft.hpp"
#include "evaluation.hpp"

void RunPerft(const std::string& fen, int depth, int iters, ui64 value)
{
	Show<ShowTypes::Console>::Op(fen);
	Bitboard& b = BitboardFromFen(fen);
    b.zobrists.push_back( ZobristFromBitboard(b) );
	aks::time::HighResStopWatch w;
	w.startTimer();
	ui64 sum=0;
	w.startTimer();
	for(int i=0; i<iters; ++i)
	{
    PerftHelper ph;
		sum += Perft(b,depth,0, ph);
    //std::cout <<"ph.mates             "<< ph.mates             << std::endl;
    //std::cout <<"ph.captures          "<< ph.captures          << std::endl;
    //std::cout <<"ph.count             "<< ph.count             << std::endl;
    //std::cout <<"ph.ep                "<< ph.ep                << std::endl;
    //std::cout <<"ph.stalemates        "<< ph.stalemates        << std::endl;
    //std::cout <<"ph.promotions        "<< ph.promotions        << std::endl;
    //std::cout <<"ph.castle_kingsides  "<< ph.castle_kingsides  << std::endl;
    //std::cout <<"ph.castle_queensides "<< ph.castle_queensides << std::endl;
	}
	w.stopTimer();
	std::string dpt = to_string(depth);
	std::string num = to_string(sum/iters);
	std::string avg = to_string(sum/w.getElapsedTime());
	Show<ShowTypes::Console>::Op(std::string("Depth: ")+dpt);
    std::cout << sum << std::endl;;
    std::cout << value << std::endl;
	std::cout << (sum == value) << std::endl;
	Show<ShowTypes::Console>::Op(w.getElapsedTime()/iters);
	Show<ShowTypes::Console>::Op(avg);

	if((sum/iters) != value)
		Show<ShowTypes::Console>::Op("XXXXXXXXXXXXXXXXXXXXXXXXX");
	Show<ShowTypes::Console>::Op("----------------------");

}

void RunZobTest(const std::string& fen, int iters)
{
    Bitboard& b = BitboardFromFen(fen);
    ui64 zob_original = ZobristFromBitboard(b);
    b.zobrists.push_back(zob_original);
    int n_moves;
    bool go_on = true;
    for(int i=0; i<iters; ++i)
    {
        if(go_on)
        {
            GeneratePseudoLegalMoves(b, b.moves_arr[i]);
            n_moves = b.moves_arr[i].size();
            for(int j=0; j<10000; ++j)
            {
                int n = (n_moves-1)%Random<ui64>();
                move m = b.moves_arr[i][n];
                if(b.MakeMove(m))
                {
                    ui64 zob_now = ZobristFromBitboard(b);
                    b.zobrists.push_back(b.zobrists.back());
                    UpdateZobristFromMove(b.zobrists.back(), m, b);
                    if((zob_now != b.zobrists.back()))
                        Show<ShowTypes::Console>::Op( "No" );
                    break;
                }
                else
                {
                    go_on = false;
                    break;
                }
            }
        }
    }
}









int NegaMax(Bitboard& b, int depth, int current_depth, ui64& nodesSeen)
{
  int best = Constants::NEG_INF;
  if(depth <= 0) { ++nodesSeen; return evaluate(b); }

  GeneratePseudoLegalMoves(b, b.moves_arr[current_depth]);
  int n_moves = b.moves_arr[current_depth].size();

  for (int i = 0; i < n_moves; i++) 
  {
      move& m =b.moves_arr[current_depth][i];
      if(b.MakeMove(m))
      {
          b.zobrists.push_back(b.zobrists.back());
          UpdateZobristFromMove(b.zobrists.back(), m, b);
          int val = -NegaMax(b, depth-1, current_depth+1, nodesSeen);
          if( val > best )
            best = val;
          b.zobrists.pop_back();
      }
      b.UnmakeMove();
  }
  return best;
}
int GetNegaMaxScore(Bitboard& b, int depth, ui64& nodesSeen)
{
  //if(b.IsWhitesTurn())
  //  return NegaMax(b,depth,0);
  //else
  //  return -NegaMax(b,depth,0);
  int val = NegaMax(b,depth,0, nodesSeen);
  return val;
}

struct PV
{
    PV():cmove(0){}
    int cmove;                           // Number of moves in the line.
    move argmove[Constants::max_depth];  // The line.
    void Show() const
    {
        for(int i=0; i<cmove; ++i)
        {
            std::cout << Sq::SSq[argmove[i].from] << Sq::SSq[argmove[i].to] << " ";
        }
    }
};

int AlphaBeta(Bitboard& b, int depth, int alpha, int beta, int current_depth, ui64& nodesSeen, PV* pv, PV* prev_PV)
{
  PV pv_local;
  if(depth <= 0)
  {
      pv->cmove = 0;
      ++nodesSeen; 
      return evaluate(b); 
  }

  int val = 0;
  bool hasLegalMoves = false;
  if(prev_PV && prev_PV->cmove > current_depth)
  {
      move& m = prev_PV->argmove[current_depth];
      bool isMoveLegal = b.MakeMove(m);
      if(isMoveLegal)
      {
          hasLegalMoves = true;
          b.zobrists.push_back(b.zobrists.back());
          UpdateZobristFromMove(b.zobrists.back(), m, b);
          val = -AlphaBeta(b, depth-1, -beta, -alpha, current_depth+1, nodesSeen, &pv_local, prev_PV);
          if( val >= beta )
          {
            b.zobrists.pop_back();
            b.UnmakeMove();
            return beta;
          }
          if( val > alpha )
          {
            alpha = val;
            pv->argmove[0] = m;
            memcpy(pv->argmove + 1, pv_local.argmove, pv_local.cmove * sizeof(move));
            pv->cmove = pv_local.cmove + 1;
          }
          b.zobrists.pop_back();
      }
      b.UnmakeMove();
  }
  
  GeneratePseudoLegalMoves(b, b.moves_arr[current_depth]);
  int n_moves = b.moves_arr[current_depth].size();
  
  for (int i = 0; i < n_moves; i++) 
  {
      move& m =b.moves_arr[current_depth][i];
      bool isMoveLegal = b.MakeMove(m);
      if(isMoveLegal)
      {
          hasLegalMoves = true;
          b.zobrists.push_back(b.zobrists.back());
          UpdateZobristFromMove(b.zobrists.back(), m, b);
          val = -AlphaBeta(b, depth-1, -beta, -alpha, current_depth+1, nodesSeen, &pv_local, 0);
          if( val >= beta )
          {
            b.zobrists.pop_back();
            b.UnmakeMove();
            return beta;
          }
          if( val > alpha )
          {
            alpha = val;
            pv->argmove[0] = m;
            memcpy(pv->argmove + 1, pv_local.argmove, pv_local.cmove * sizeof(move));
            pv->cmove = pv_local.cmove + 1;
          }
          b.zobrists.pop_back();
      }
      b.UnmakeMove();
  }

  if(!hasLegalMoves)
  {
      if(b.IsKingInCheck())
      {
          int mating_value = Constants::MATE_VALUE - current_depth;
          if (mating_value < beta) 
          {
              beta = mating_value;
              if (alpha >= mating_value) 
              {
                  return mating_value;
              }
          }
          mating_value = -Constants::MATE_VALUE + current_depth;

          if (mating_value > alpha) 
          {
              alpha = mating_value;
              if (beta <= mating_value) 
              {
                  return mating_value;
              }
          }
      }
      else
      {
          return Constants::DRAW_VALUE;
          //int draw_value = Constants::DRAW_VALUE;
          //if (draw_value < beta) 
          //{
          //    beta = draw_value;
          //    if (alpha >= draw_value) 
          //    {
          //        return draw_value;
          //    }
          //}
          //draw_value = -Constants::MATE_VALUE;

          //if (draw_value > alpha) 
          //{
          //    alpha = draw_value;
          //    if (beta <= draw_value) 
          //    {
          //        return draw_value;
          //    }
          //}
      }
  }

  return alpha;
}
int GetAlphaBetaScore(Bitboard& b, int depth, ui64& nodesSeen, PV& pv, PV& prev_PV)
{
  //if(b.IsWhitesTurn())
  //  return NegaMax(b,depth,0);
  //else
  //  return -NegaMax(b,depth,0);
  int val = AlphaBeta(b,depth,Constants::NEG_INF,Constants::POS_INF,0,nodesSeen, &pv, &prev_PV);
  return val;
}

void CompareMiniMax_AlphaBeta(const std::string& fen, int depth)
{
  ui64 nodesSeen = 0;
  ui64 nodesSeenAB = 0;
  double mmTime = 0;
  double abTime = 0;
  aks::time::HighResStopWatch w;
	w.startTimer();
  //{
  //  Bitboard& b = BitboardFromFen(fen);
  //  b.zobrists.push_back( ZobristFromBitboard(b) );
  //  Show<ShowTypes::Console>::Op(GetNegaMaxScore(b, depth, nodesSeen));
  //}
  w.stopTimer();
  mmTime = w.getElapsedTime();
  w.startTimer();
  {
    PV pv;
    PV prev_pv;
    Bitboard& b = BitboardFromFen(fen);
    b.zobrists.push_back( ZobristFromBitboard(b) );
    int val = GetAlphaBetaScore(b, depth, nodesSeenAB, pv, prev_pv);
    Show<ShowTypes::Console>::Op(b);
    for(int i=0; i<pv.cmove; ++i)
    {
        //Show<ShowTypes::Console>::Op("----------");
        b.MakeMove(pv.argmove[i]);
        Show<ShowTypes::Console>::Op(b);
        Show<ShowTypes::Console>::Op(FenFromBitboard(b));
    }
    Show<ShowTypes::Console>::Op(val * ( b.IsWhitesTurn()?1:1));
  }
  w.stopTimer();
  abTime = w.getElapsedTime();
  Show<ShowTypes::Console>::Op((long)nodesSeen);
  Show<ShowTypes::Console>::Op((long)nodesSeenAB);
  Show<ShowTypes::Console>::Op((double)nodesSeenAB/nodesSeen);
  Show<ShowTypes::Console>::Op(mmTime);
  Show<ShowTypes::Console>::Op(abTime);
  Show<ShowTypes::Console>::Op(abTime/mmTime);
  Show<ShowTypes::Console>::Op(nodesSeen/mmTime);
  Show<ShowTypes::Console>::Op(nodesSeenAB/abTime);
  Show<ShowTypes::Console>::Op("------------------");
}

int absVal(int i){ return i<0?-i:i; }

void PlayGame(const std::string& fen)
{
    const std::string filename("gamefile_19.txt"); 
    Bitboard& b = BitboardFromFen(fen);
    b.zobrists.push_back( ZobristFromBitboard(b) );
    Show<ShowTypes::Console>::Op(b);
    Show<ShowTypes::File>::Op(filename, b);
    bool goOn = true;
    for(int n=0; n<300; ++n)
    {
        if(goOn)
        {
            aks::time::HighResStopWatch w;
            int val = 0;
            int depth = 0;
            move bestMove;
            double totalTime = 0;
            double max_time = 10.0;//((n%2)==0) ? 5.0 : 0.25;
            type_array<PV, Constants::max_depth>* pvs = new type_array<PV, Constants::max_depth>();
            pvs->push_back();
            while(totalTime < max_time && ((depth + absVal(val)) < Constants::MATE_VALUE))
            {
                PV& prev_pv = (*pvs)[depth];

                ++depth;
                pvs->push_back();
                PV& pv = (*pvs)[depth];
                
                ui64 nodesSeen = 0;
                w.startTimer();
                val = GetAlphaBetaScore(b, depth, nodesSeen, pv, prev_pv);  
                w.stopTimer();
                totalTime += w.getElapsedTime();
                if(max_time - totalTime < w.getElapsedTime())
                {
                    totalTime = max_time;
                }

                if( absVal(val) == Constants::MATE_VALUE )
                {
                    std::cout << (b.IsWhitesTurn() ? "White Mated" : "Black Mated") << std::endl;
                    goOn = false;
                    break;
                }
                
                std::cout << (b.IsWhitesTurn()? val:-val) << "\t" << nodesSeen << "\t\t" << w.getElapsedTime() << "\t\t";
                pv.Show();
                std::cout << std::endl;
                bestMove = pv.argmove[0];
            }
            delete pvs;
            if(goOn)
            {
                b.MakeMove(bestMove);
                b.zobrists.push_back(b.zobrists.back());
                UpdateZobristFromMove(b.zobrists.back(), bestMove, b);
                Show<ShowTypes::Console>::Op(b);
                Show<ShowTypes::File>::Op(filename, b);
                for(int i=0; i<b.moves.size(); ++i)
                {
                    std::cout << Sq::SSq[b.moves[i].from] << Sq::SSq[b.moves[i].to] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

}

int main()
{
  //Show<ShowTypes::Console>::Op((unsigned)sizeof(Bitboard));
  //Show<ShowTypes::Console>::Op((unsigned)sizeof(move));
  //Show<ShowTypes::Console>::Op((unsigned)sizeof(ui64));
  //Show<ShowTypes::Console>::Op((unsigned)sizeof(move_packed));
  //Show<ShowTypes::Console>::Op((unsigned)sizeof(TT_Entry));
  //Show<ShowTypes::Console>::Op((unsigned)sizeof(TT_Big_Entry));

	init_all();
	
	bool RunPerfts = false;

	if(1)
  {
    //const std::string fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const std::string fen("r1r2k2/6bQ/p2pp1p1/1pqP2P1/8/8/PPn5/1KBR3R b - - 0 1");
      //const std::string fen("8/k4ppp/8/K4PPP/8/8/8/8 w - - 0 1");
      //const std::string fen("8/8/8/8/ppp4k/8/PPP4K/8 b - - 0 1");
    PlayGame(fen);
    //CompareMiniMax_AlphaBeta(fen, 1);
    //CompareMiniMax_AlphaBeta(fen, 2);
    //CompareMiniMax_AlphaBeta(fen, 3);
    //CompareMiniMax_AlphaBeta(fen, 4);
    //CompareMiniMax_AlphaBeta(fen, 5);
    //CompareMiniMax_AlphaBeta(fen, 6);
    //Bitboard& b = BitboardFromFen(fen);
    //Show<ShowTypes::Console>::Op(evaluate(b));
    //RunZobTest(fen, 100);
		//RunPerft(fen,  1, 1, 20);
		//RunPerft(fen,  2, 1, 400);
		//RunPerft(fen,  3, 1, 8902);
    	//RunPerft(fen,  4, 1, 197281);
		//RunPerft(fen,  5, 1, 4865609);
		// RunPerft(fen,  6, 1, 119060324);
		// RunPerft(fen,  7, 1, 3195901860);
		// RunPerft(fen,  8, 1, 84998978956);
		// RunPerft(fen,  9, 1, 2439530234167);
		// RunPerft(fen, 10, 1, 69352859712417);
  }
  if(0)
  {
    //const std::string fen("2k5/4P3/4K3/8/8/8/8/8/8 w - - 0 1");
    //const std::string fen("8/k7/2K5/1Q6/8/8/8/8 w - - 0 5");
    const std::string fen("8/kQ6/2K5/8/8/8/8/8 b - - 0 5");
    
    //const std::string fen("3brrb1/2N4B/8/2p4Q/2p2k2/5P2/4P1KR/2N2RB1 w - - 0 1");
    //const std::string fen("8/5p1k/6pp/3p4/Rpp2n1P/5P2/PbP2r1P/3K4 w - - 1 33");
    //const std::string fen("4k3/5R2/4PpN1/8/8/4pPn1/5r2/4K3 b - - 0 1");
    //const std::string fen("6k1/1p3Rpp/p1pN4/2P1n3/1PB5/7P/1q4PK/8 w - - 0 1");
    //const std::string fen("5K2/8/3p2k1/3P1R2/2P2p1P/2B2P2/8/8 w - - 0 1");
    //const std::string fen("4r3/7q/nb2prRp/p2p3P/B1kP4/P7/1P2N1P1/1K3N2 w - - 2 2");

    CompareMiniMax_AlphaBeta(fen, 1);

    
	//RunPerft(fen, 1, 1, 20);
	//RunPerft(fen, 2, 1, 400);
	//RunPerft(fen, 3, 1, 8902);
	//RunPerft(fen, 4, 1, 197281);
	//RunPerft(fen, 5, 1, 2826);
    //RunPerft(fen, 8, 1, 10000000000000);
	//RunPerft(fen, 2, 1,  195314821);
	//RunPerft(fen, 3, 1,  195314821);
	//RunPerft(fen, 4, 1,  195314821);
	//RunPerft(fen, 5, 1,  195314821);
	//RunPerft(fen, 6, 1,  195314821);
	//RunPerft(fen, 7, 1,  195314821);
	//RunPerft(fen, 9, 1,  195314821);
    //RunPerft(fen, 11, 1,  195314821);
  }
  if(0)
  {
    const std::string fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    //    Bitboard& b = BitboardFromFen(fen);
    //Show<ShowTypes::Console>::Op(evaluate(b));
	//RunPerft(fen, 1, 1, 48);
	//RunPerft(fen, 2, 1, 2039);
	//RunPerft(fen, 3, 1, 97862);
	//RunPerft(fen, 4, 1, 4085603);
    CompareMiniMax_AlphaBeta(fen, 4);
	//RunPerft(fen, 5, 1, 193690690);
	//RunPerft(fen, 6, 1, 8031647685);

  }
  if(RunPerfts)
  {
    const std::string fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
	//RunPerft(fen, 1, 1, 14);
	//RunPerft(fen, 2, 1, 191);
	//RunPerft(fen, 3, 1, 2812);
	//RunPerft(fen, 4, 1, 43238);
	RunPerft(fen, 5, 1, 674624);
	//RunPerft(fen, 6, 1, 11030083);
  }
  if(RunPerfts)
  {
    const std::string fen("8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - - 1 67");
	//RunPerft(fen, 1, 1, 50);
	//RunPerft(fen, 2, 1, 279);
  }
    if(0)
  {
    const std::string fen("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");

    CompareMiniMax_AlphaBeta(fen, 12);
	//RunPerft(fen, 1, 1, 24);
	//RunPerft(fen, 2, 1, 496);
	//RunPerft(fen, 3, 1, 9483);
	//RunPerft(fen, 4, 1, 182838);
	//RunPerft(fen, 5, 1, 3605103);
	//RunPerft(fen, 6, 1, 71179139);
  }
  if(RunPerfts)
  {
    const std::string fen("rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
	  //RunPerft(fen, 5, 1, 11139762);
  }
    if(RunPerfts)
  {
    const std::string fen("8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
	//RunPerft(fen, 6, 1, 38633283);
  }

    //RunZobTest("4k3/8/8/8/8/8/4P3/4K3 w KQkq - 0 1", 10);
    //RunZobTest("r1bqkb1r/5ppp/p1np1n2/3Np1B1/1p2P3/NP6/P1P2PPP/R2QKB1R b KQkq - 0 11", 100);
    //RunZobTest("4k3/8/8/8/8/3p4/4P3/4K3 w - - 0 1", 10);

    return 0;
}
