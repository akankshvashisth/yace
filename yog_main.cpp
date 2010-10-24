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

void RunPerft(const std::string& fen, int depth, int iters, ui64 value)
{
	Show<ShowTypes::Console>::Op(fen);
	Bitboard& b = BitboardFromFen(fen);
	//for(int i=0; i<Constants::max_depth; ++i)
	//	b.moves_arr[i].reserve(128);
	aks::time::HighResStopWatch w;
	w.startTimer();
	ui64 sum=0;
	w.startTimer();
	for(unsigned int i=0; i<iters; ++i)
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

    ui64 n_moves;
    bool go_on = true;
    for(int i=0; i<iters; ++i)
    {
        if(go_on)
        {
            GeneratePseudoLegalMoves(b, b.moves_arr[i]);
            n_moves = b.moves_arr[i].size();
            for(int j=0; j<10000; ++j)
            {
                ui64 n = (n_moves-1)%Random<ui64>();
                move m = b.moves_arr[i][n];
                if(b.MakeMove(m))
                {
                    ui64 zob_now = ZobristFromBitboard(b);
                    UpdateZobristFromMove(zob_original, m, b);
                    if((zob_now != zob_original))
                        Show<ShowTypes::Console>::Op( "No" );
                    //Show<ShowTypes::Console>::Op( b );
                    //Show<ShowTypes::Console>::Op( FenFromBitboard(b) );
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

int main()
{
    //Bitboard b;
    //b.PutPieceAt(PieceType::wknights, Sq::a1);
    //b.PutPieceAt(PieceType::wbishops, Sq::h1);
    //b.PutPieceAt(PieceType::bknights, Sq::a8);
    //b.PutPieceAt(PieceType::bbishops, Sq::h8);
    //Show<ShowTypes::Console>::Op(b);
    ////Show<ShowTypes::Console>::Op( KnightAttacks( KnightAttacks( KnightAttacks( KnightAttacks( SqSetBit(Sq::h2) ) ) ) ) );
    //BB h2 = SqSetBit(Sq::h2);
    //BB a3 = SqSetBit(Sq::a3);
    //BB wp = h2 | a3;
    //BB b6 = SqSetBit(Sq::b6);
    //BB c4 = SqSetBit(Sq::c4);
    //BB bp = b6 | c4;

    //BB wbb;
    //BB bbb;

    ////for( int i=0; i<7; ++i )
    //{
    //    /*Show<ShowTypes::Console>::Op(KnightFill(bb));
    //    bb = KnightAttacks(bb);*/
    //    /*bb = KingAttacks(bb);
    //    Show<ShowTypes::Console>::Op(bb);*/
    //    //bb = WhitePawnAnyAttacks(bb);
    //    wbb = WhiteSafePawnSquares(wp,bp);
    //    bbb = BlackSafePawnSquares(wp,bp);
    //    Show<ShowTypes::Console>::Op(wbb);
    //    Show<ShowTypes::Console>::Op("------------------");
    //    Show<ShowTypes::Console>::Op(bbb);
    //    Show<ShowTypes::Console>::Op("------------------");
    //}

	//Show<ShowTypes::Console>::Op((unsigned)(sizeof(Bitboard)));
	//Show<ShowTypes::Console>::Op((unsigned)(sizeof(move_array<Constants::max_game_length>)));
	//Show<ShowTypes::Console>::Op((unsigned)(Constants::max_depth * sizeof(move_array<Constants::max_moves_per_position>)));

    //FEN::FenParser("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //FEN::FenParser("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    //FEN::FenParser("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    //FEN::FenParser("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    //FEN::FenParser("r1bqkb1r/5ppp/p1np1n2/1p1Np1B1/4P3/N7/PPP2PPP/R2QKB1R b KQkq - 0 10");

    //const std::string fen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    //Bitboard b;
    ////Show<ShowTypes::Console>::Op(b);
    //b = BitboardFromFen(fen);
    //const std::string fenFromBoard = FenFromBitboard(b);
    ////Show<ShowTypes::Console>::Op(b);
    //Show<ShowTypes::Console>::Op(fen);
    //Show<ShowTypes::Console>::Op(fenFromBoard);
    //Show<ShowTypes::Console>::Op(FEN::FenParser(fen));
    //Show<ShowTypes::Console>::Op(b);

	init_all();
	
	bool RunPerfts = true;

	if(RunPerfts)
  {
    const std::string fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//RunPerft(fen, 1, 1, 20);
	//RunPerft(fen, 2, 1, 400);
	//RunPerft(fen, 3, 1, 8902);
	//RunPerft(fen, 4, 1, 197281);
	RunPerft(fen, 5, 1, 4865609);
	//RunPerft(fen, 6, 1, 119060324);
  }
  if(RunPerfts)
  {
    const std::string fen("4k3/8/4P3/8/8/8/8/4K3 w KQkq - 0 1");
	//RunPerft(fen, 1, 1, 20);
	//RunPerft(fen, 2, 1, 400);
	//RunPerft(fen, 3, 1, 8902);
	//RunPerft(fen, 4, 1, 197281);
	//RunPerft(fen, 5, 1, 4865609);
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
  if(RunPerfts)
  {
    const std::string fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
	//RunPerft(fen, 1, 1, 48);
	//RunPerft(fen, 2, 1, 2039);
	//RunPerft(fen, 3, 1, 97862);
	//RunPerft(fen, 4, 1, 4085603);
	RunPerft(fen, 5, 1, 193690690);
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
    if(RunPerfts)
  {
    const std::string fen("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");

	//RunPerft(fen, 1, 1, 24);
	//RunPerft(fen, 2, 1, 496);
	//RunPerft(fen, 3, 1, 9483);
	//RunPerft(fen, 4, 1, 182838);
	RunPerft(fen, 5, 1, 3605103);
	//RunPerft(fen, 6, 1, 71179139);
  }
  if(RunPerfts)
  {
    const std::string fen("rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
	RunPerft(fen, 5, 1, 11139762);
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
