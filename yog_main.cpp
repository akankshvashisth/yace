#include "bitboard.hpp"
#include "show.hpp"
#include "bbfunctions.hpp"
#include "fen_parser.hpp"
#include "random.hpp"
#include "bitboard_fen.hpp"
#include "lookup.hpp"
#include "piece_attacks.hpp"
#include "move_gen.hpp"

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

    //Show<ShowTypes::Console>::Op(sizeof(ui64));

    //FEN::FenParser("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //FEN::FenParser("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    //FEN::FenParser("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    //FEN::FenParser("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    //FEN::FenParser("r1bqkb1r/5ppp/p1np1n2/1p1Np1B1/4P3/N7/PPP2PPP/R2QKB1R b KQkq - 0 10");

    //const std::string fen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    //Bitboard b;
    ////Show<ShowTypes::Console>::Op(b);
    //b = BitboardFromFen(fen);
    const std::string fenFromBoard = FenFromBitboard(b);
    ////Show<ShowTypes::Console>::Op(b);
    //Show<ShowTypes::Console>::Op(fen);
    //Show<ShowTypes::Console>::Op(fenFromBoard);
    //Show<ShowTypes::Console>::Op(FEN::FenParser(fen));
    //Show<ShowTypes::Console>::Op(b);

	init_all();

	//for(unsigned i=0; i<64; ++i)
	//{
	//	Show<ShowTypes::Console>::Op(lookup::direction_attacks[Dir::SE][i]);
	//	Show<ShowTypes::Console>::Op("--------");
	//}


	//Show<ShowTypes::Console>::Op( QueenAttacks((Constants::file_d|Constants::rank_5), Sq::g2) );

	if(0)
  {
    const std::string fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Bitboard b = BitboardFromFen(fen);

    Show<ShowTypes::Console>::Op(Perft(b,1));
    Show<ShowTypes::Console>::Op("20");
    Show<ShowTypes::Console>::Op(Perft(b,2));
    Show<ShowTypes::Console>::Op("400");
    Show<ShowTypes::Console>::Op(Perft(b,3));
    Show<ShowTypes::Console>::Op("8902");
    Show<ShowTypes::Console>::Op(Perft(b,4));
    Show<ShowTypes::Console>::Op("197281");
    //working- Show<ShowTypes::Console>::Op(Perft(b,5));
    //working- Show<ShowTypes::Console>::Op("4865609");
    //working- Show<ShowTypes::Console>::Op(Perft(b,6));
    //working- Show<ShowTypes::Console>::Op("119060324");
  }
  
  if(1)
  {
    const std::string fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Bitboard b = BitboardFromFen(fen);

    Show<ShowTypes::Console>::Op(Perft(b,1));
    Show<ShowTypes::Console>::Op("48");
    //Show<ShowTypes::Console>::Op(Perft(b,2));
    //Show<ShowTypes::Console>::Op("2039");
    //Show<ShowTypes::Console>::Op(Perft(b,3));
    //Show<ShowTypes::Console>::Op("8902");
    //Show<ShowTypes::Console>::Op(Perft(b,4));
    //Show<ShowTypes::Console>::Op("197281");
    //Show<ShowTypes::Console>::Op(Perft(b,5));
    //Show<ShowTypes::Console>::Op("4865609");
    //Show<ShowTypes::Console>::Op(Perft(b,6));
    //Show<ShowTypes::Console>::Op("119060324");
  }
  
  if(0)
  {
    const std::string fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    Bitboard b = BitboardFromFen(fen);

    Show<ShowTypes::Console>::Op(Perft(b,1));
    Show<ShowTypes::Console>::Op("14");
    Show<ShowTypes::Console>::Op(Perft(b,2));
    Show<ShowTypes::Console>::Op("191");
    Show<ShowTypes::Console>::Op(Perft(b,3));
    Show<ShowTypes::Console>::Op("2812");
    Show<ShowTypes::Console>::Op(Perft(b,4));
    Show<ShowTypes::Console>::Op("43238");
    Show<ShowTypes::Console>::Op(Perft(b,5));
    Show<ShowTypes::Console>::Op("674624");
    Show<ShowTypes::Console>::Op(Perft(b,6));
    Show<ShowTypes::Console>::Op("11030083");
  }

    if(0)
  {
    const std::string fen("8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - - 1 67");
    Bitboard b = BitboardFromFen(fen);

    Show<ShowTypes::Console>::Op(Perft(b,1));
    Show<ShowTypes::Console>::Op("50");
    Show<ShowTypes::Console>::Op(Perft(b,2));
    Show<ShowTypes::Console>::Op("279");
  }
     
    if(0)
  {
    const std::string fen("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    Bitboard b = BitboardFromFen(fen);

    //std::vector<move> mvsp = GeneratePseudoLegalMoves(b);
    //std::vector<move> mvsl = GenerateLegalMoves(b);
    //std::vector<move> mvsi = GenerateIllegalMoves(b);
    //std::vector<std::pair<move,int> >div = Divide(b,2);
    Show<ShowTypes::Console>::Op(Perft(b,1));
    Show<ShowTypes::Console>::Op("24");
    Show<ShowTypes::Console>::Op(Perft(b,2));
    Show<ShowTypes::Console>::Op("496");
    Show<ShowTypes::Console>::Op(Perft(b,3));
    Show<ShowTypes::Console>::Op("9483");
    Show<ShowTypes::Console>::Op(Perft(b,4));
    Show<ShowTypes::Console>::Op("182838");
    Show<ShowTypes::Console>::Op(Perft(b,5));
    Show<ShowTypes::Console>::Op("3605103");
    Show<ShowTypes::Console>::Op(Perft(b,6));
    Show<ShowTypes::Console>::Op("71179139");
  }

  if(0)
  {
    const std::string fen("rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
    Bitboard b = BitboardFromFen(fen);

    Show<ShowTypes::Console>::Op(Perft(b,5));
    Show<ShowTypes::Console>::Op("11139762");
  }

    if(1)
  {
    const std::string fen("8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
    Bitboard b = BitboardFromFen(fen);

    Show<ShowTypes::Console>::Op(Perft(b,6));
    Show<ShowTypes::Console>::Op("38633283");
  }


	//for(int i=0; i<mvs.size(); ++i)
	//{

	//}


    return 0;
}
