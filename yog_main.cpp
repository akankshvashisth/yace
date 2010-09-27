#include "bitboard.hpp"
#include "show.hpp"
#include "bbfunctions.hpp"
#include "fen_parser.hpp"
#include "random.hpp"
#include "bitboard_fen.hpp"

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

    const std::string fen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    Bitboard b;
    //Show<ShowTypes::Console>::Op(b);
    b = BitboardFromFen(fen);
    const std::string fenFromBoard = FenFromBitboard(b);
    //Show<ShowTypes::Console>::Op(b);
    Show<ShowTypes::Console>::Op(fen);
    Show<ShowTypes::Console>::Op(fenFromBoard);
    //Show<ShowTypes::Console>::Op(FEN::FenParser(fen));
    //Show<ShowTypes::Console>::Op(b);

    return 0;
}
