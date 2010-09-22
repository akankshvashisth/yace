#include "bitboard.hpp"
#include "show.hpp"
#include "bbfunctions.hpp"

int main()
{
	Bitboard b;
	b.PutPieceAt(PieceType::wknights, Sq::a1);
	b.PutPieceAt(PieceType::wbishops, Sq::h1);
	b.PutPieceAt(PieceType::bknights, Sq::a8);
	b.PutPieceAt(PieceType::bbishops, Sq::h8);
	Show<ShowTypes::Console>::Op(b);
	//Show<ShowTypes::Console>::Op( KnightAttacks( KnightAttacks( KnightAttacks( KnightAttacks( SqSetBit(Sq::h2) ) ) ) ) );
	BB h2 = SqSetBit(Sq::h2);
	BB a3 = SqSetBit(Sq::a3);
	BB wp = h2 | a3;
	BB b6 = SqSetBit(Sq::b6);
	BB c4 = SqSetBit(Sq::c4);
	BB bp = b6 | c4;

	BB wbb;
	BB bbb;

	//for( int i=0; i<7; ++i )
	{
		/*Show<ShowTypes::Console>::Op(KnightFill(bb));
		bb = KnightAttacks(bb);*/
		/*bb = KingAttacks(bb);
		Show<ShowTypes::Console>::Op(bb);*/
		//bb = WhitePawnAnyAttacks(bb);
		wbb = WhiteSafePawnSquares(wp,bp);
		bbb = BlackSafePawnSquares(wp,bp);
		Show<ShowTypes::Console>::Op(wbb);
		Show<ShowTypes::Console>::Op("------------------");
		Show<ShowTypes::Console>::Op(bbb);
		Show<ShowTypes::Console>::Op("------------------");
	}
	return 0;
}