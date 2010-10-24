#ifndef __move_gen_hpp
#define __move_gen_hpp

#include "bitboard.hpp"
#include "bbfunctions.hpp"
#include "bitfunctions.hpp"
#include "piece_attacks.hpp"
#include "lookup.hpp"
#include "enums.hpp"
#include "move.hpp"
#include "show.hpp"
#include "bitboard_zobrist.hpp"


//#include "show.hpp"

#include <utility>
#include <vector>

move_array<Constants::max_moves_per_position>& GeneratePseudoLegalMoves( Bitboard& bb, move_array<Constants::max_moves_per_position>& mvs )
{
	mvs.clear();
	//mvs.reserve(64);

	/*std::vector<move> mvs;*/
	const bool isWhitesTurn = bb.IsWhitesTurn();
	ui64 pawns, bishops, rooks, queens, knights, king, opponent, self, opponentOrEp;
	ui64 empty = (bb.PiecesAt(PieceType::none));
	ui64 all = bb.PiecesAt(PieceType::all);
	PieceType::EPieceType epawns, ebishops, erooks, equeens, eknights, eking, eopponent, eself;
	const  Sq::ESq epSq = bb.EpSquare();
	const ui64 epSqBB = lookup::single_bit_set[epSq];
	/*const bool hasEp = epSq != Sq::none;*/

	move m;
	//m.special = MoveType::s_none;
	//m.isKingSideCastle = false;
	//m.isQueenSideCastle = false;

	if(isWhitesTurn)
	{
		pawns   = bb.PiecesAt(PieceType::wpawns);
		bishops = bb.PiecesAt(PieceType::wbishops);
		rooks   = bb.PiecesAt(PieceType::wrooks);
		queens  = bb.PiecesAt(PieceType::wqueens);
		knights = bb.PiecesAt(PieceType::wknights);
		king    = bb.PiecesAt(PieceType::wking);
		opponent= bb.PiecesAt(PieceType::black);
		self    = bb.PiecesAt(PieceType::white);

		epawns   = PieceType::wpawns	;
		ebishops = PieceType::wbishops	;
		erooks   = PieceType::wrooks	;
		equeens  = PieceType::wqueens	;
		eknights = PieceType::wknights	;
		eking    = PieceType::wking		;
		eopponent= PieceType::black		;
		eself    = PieceType::white		;
	}
	else
	{
		pawns   = bb.PiecesAt(PieceType::bpawns);
		bishops = bb.PiecesAt(PieceType::bbishops);
		rooks   = bb.PiecesAt(PieceType::brooks);
		queens  = bb.PiecesAt(PieceType::bqueens);
		knights = bb.PiecesAt(PieceType::bknights);
		king    = bb.PiecesAt(PieceType::bking);
		opponent= bb.PiecesAt(PieceType::white);
		self    = bb.PiecesAt(PieceType::black);

		epawns   = PieceType::bpawns	;
		ebishops = PieceType::bbishops	;
		erooks   = PieceType::brooks	;
		equeens  = PieceType::bqueens	;
		eknights = PieceType::bknights	;
		eking    = PieceType::bking		;
		eopponent= PieceType::white		;
		eself    = PieceType::black		;
	}

	opponentOrEp = opponent | epSqBB;

	const ui64 pcnt = PopulationCount(pawns  ); 
	const ui64 bcnt = PopulationCount(bishops);   
	const ui64 rcnt = PopulationCount(rooks  ); 
	const ui64 qcnt = PopulationCount(queens ); 
	const ui64 ncnt = PopulationCount(knights); 
	//const ui64 kcnt = PopulationCount(king   ); 

	{
		m.piece = epawns;
		if(isWhitesTurn)
		{
			for(unsigned i=0; i<pcnt; ++i)
			{
				m.special = MoveType::s_none;

				Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(pawns);
				m.from = sq;
				const ui64 sqBB = lookup::single_bit_set[sq];
				pawns &= (~sqBB);
				ui64 normal = WhiteSinglePawnPushTargets(sqBB, empty);
				if(sq & Constants::not_rank_2)
					normal |= WhiteDoublePawnPushTargets(sqBB, empty);
				const ui64 norcnt    = PopulationCount(normal);

				m.type = MoveType::normal;
				for(unsigned j=0; j<norcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
					normal &= (lookup::not_single_bit_set[to]);
					m.to = to;
					if( lookup::single_bit_set[to] & Constants::rank_8 )
					{
						m.special = MoveType::promotion;
						m.promoted = PieceType::wqueens;
						mvs.push_back(m);
						m.promoted = PieceType::wrooks;
						mvs.push_back(m);
						m.promoted = PieceType::wbishops;
						mvs.push_back(m);
						m.promoted = PieceType::wknights;
						mvs.push_back(m);
					}
					else
					{
						m.special = MoveType::s_none;
						mvs.push_back(m);
					}
				}

				ui64 captures = WhitePawnAnyAttacks (sqBB) &  opponentOrEp;
				const ui64 capcnt    = PopulationCount(captures);

				m.type = MoveType::capture;
				for(unsigned j=0; j<capcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
					captures &= (lookup::not_single_bit_set[to]);
					m.to = to;
					m.captured = bb.PieceAtSq(to);

					if( lookup::single_bit_set[to] & Constants::rank_8 )
					{
						m.special = MoveType::promotion;
						m.promoted = PieceType::wqueens;
						mvs.push_back(m);
						m.promoted = PieceType::wrooks;
						mvs.push_back(m);
						m.promoted = PieceType::wbishops;
						mvs.push_back(m);
						m.promoted = PieceType::wknights;
						mvs.push_back(m);
					}
					else
					{
						m.special = (to == epSq) ? MoveType::ep : MoveType::s_none;
						mvs.push_back(m);
					}
				}
			}
		}
		else
		{
			for(unsigned i=0; i<pcnt; ++i)
			{
				m.special = MoveType::s_none;

				Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(pawns);
				m.from = sq;
				const ui64 sqBB = lookup::single_bit_set[sq];
				pawns &= (~sqBB);
				ui64 normal = BlackSinglePawnPushTargets(sqBB, empty);
				if(sq & Constants::not_rank_7)
					normal |= BlackDoublePawnPushTargets(sqBB, empty);
				const ui64 norcnt    = PopulationCount(normal);

				m.type = MoveType::normal;
				for(unsigned j=0; j<norcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
					normal &= (lookup::not_single_bit_set[to]);
					m.to = to;
					if( lookup::single_bit_set[to] & Constants::rank_1 )
					{
						m.special = MoveType::promotion;
						m.promoted = PieceType::bqueens;
						mvs.push_back(m);
						m.promoted = PieceType::brooks;
						mvs.push_back(m);
						m.promoted = PieceType::bbishops;
						mvs.push_back(m);
						m.promoted = PieceType::bknights;
						mvs.push_back(m);
					}
					else
					{
						m.special = MoveType::s_none;
						mvs.push_back(m);
					}
				}
				ui64 captures = BlackPawnAnyAttacks (sqBB) & opponentOrEp;
				const ui64 capcnt    = PopulationCount(captures);
				m.type = MoveType::capture;
				for(unsigned j=0; j<capcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
					captures &= (lookup::not_single_bit_set[to]);
					m.to = to;
					m.captured = bb.PieceAtSq(to);
					if( lookup::single_bit_set[to] & Constants::rank_1 )
					{
						m.special = MoveType::promotion;
						m.promoted = PieceType::bqueens;
						mvs.push_back(m);
						m.promoted = PieceType::brooks;
						mvs.push_back(m);
						m.promoted = PieceType::bbishops;
						mvs.push_back(m);
						m.promoted = PieceType::bknights;
						mvs.push_back(m);
					}
					else
					{
						m.special = (to == epSq) ? MoveType::ep : MoveType::s_none;
						mvs.push_back(m);
					}
				}
			}
		}
	}

	//m.isEp = false;
	//m.isPromotion = false;
	//m.promoted = PieceType::none;
	m.special = MoveType::s_none;

	m.piece = ebishops;
	for(unsigned i=0; i<bcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(bishops);
		m.from = sq;
		bishops &= (lookup::not_single_bit_set[sq]);
		const ui64 pmvs = BishopAttacks(bb.PiecesAt(PieceType::all), sq);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		m.type = MoveType::capture;
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
			captures &= (lookup::not_single_bit_set[to]);
			m.to = to;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		m.type = MoveType::normal;
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
			normal &= (lookup::not_single_bit_set[to]);
			m.to = to;
			mvs.push_back(m);
		}
	}

	m.piece = erooks;
	for(unsigned i=0; i<rcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(rooks);
		m.from = sq;
		rooks &= (lookup::not_single_bit_set[sq]);
		const ui64 pmvs = RookAttacks(bb.PiecesAt(PieceType::all), sq);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		m.type = MoveType::capture;
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
			captures &= (lookup::not_single_bit_set[to]);
			m.to = to;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		m.type = MoveType::normal;
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
			normal &= (lookup::not_single_bit_set[to]);
			m.to = to;
			mvs.push_back(m);
		}
	}

	m.piece = equeens;
	for(unsigned i=0; i<qcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(queens);
		m.from = sq;
		queens &= (lookup::not_single_bit_set[sq]);
		const ui64 pmvs = QueenAttacks(bb.PiecesAt(PieceType::all), sq);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		m.type = MoveType::capture;
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
			captures &= (lookup::not_single_bit_set[to]);
			m.to = to;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		m.type = MoveType::normal;
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
			normal &= (lookup::not_single_bit_set[to]);
			m.to = to;
			mvs.push_back(m);
		}
	}

	m.piece = eknights;
	for(unsigned i=0; i<ncnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(knights);
		m.from = sq;
		const ui64 sqBB = lookup::single_bit_set[sq];
		knights &= (~sqBB);
		const ui64 pmvs = KnightAttacks(sqBB);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty) ;
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		m.type = MoveType::capture;
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
			captures &= (lookup::not_single_bit_set[to]);
			m.to = to;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		m.type = MoveType::normal;
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
			normal &= (lookup::not_single_bit_set[to]);
			m.to = to;
			mvs.push_back(m);
		}
	}

	m.piece = eking;
	//for(unsigned i=0; i<kcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward_bsf(king);
		m.from = sq;
		//king &= (~lookup::single_bit_set[sq]);
		const ui64 pmvs = KingAttacks(lookup::single_bit_set[sq]);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		m.type = MoveType::capture;
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(captures);
			captures &= (lookup::not_single_bit_set[to]);
			m.to = to;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		m.type = MoveType::normal;
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward_bsf(normal);
			normal &= (lookup::not_single_bit_set[to]);
			m.to = to;
			mvs.push_back(m);
		}
		if(bb.IsWhitesTurn())
		{
			if( bb.WhiteCanCastleKingSide() && bb.IsCastleKingsideLegal() ) 
			{
                //m.to = Sq::g1;
				m.special = MoveType::castle_kingside;
				mvs.push_back(m);
				m.special = MoveType::s_none;
			}
			if( bb.WhiteCanCastleQueenSide() && bb.IsCastleQueensideLegal() )
			{
                //m.to = Sq::c1;
				m.special = MoveType::castle_queenside;
				mvs.push_back(m);
			}
		}
		else
		{
			if( bb.BlackCanCastleKingSide() && bb.IsCastleKingsideLegal() ) 
			{
                //m.to = Sq::g8;
				m.special = MoveType::castle_kingside;
				mvs.push_back(m);
				m.special = MoveType::s_none;
			}
			if( bb.BlackCanCastleQueenSide() && bb.IsCastleQueensideLegal() )
			{
                //m.to = Sq::c8;
				m.special = MoveType::castle_queenside;
				mvs.push_back(m);
			}
		}
	}
	return mvs;
}


//std::vector<move>& GenerateLegalMoves( Bitboard& bb, std::vector<move>& mvs, std::vector<move>& legal )
//{
//	mvs = GeneratePseudoLegalMoves(bb, mvs);
//	legal.clear();
//
//	for( unsigned i=0; i<mvs.size(); ++i )
//	{
//		Bitboard bb_local(bb);
//		if(bb_local.MakeMove(mvs[i]))
//		{
//			legal.push_back(mvs[i]);
//		}
//	}
//	return legal;
//}
//std::vector<move> GenerateIllegalMoves( Bitboard& bb, std::vector<move>& mvs, std::vector<move>& notlegal )
//{
//	mvs = GeneratePseudoLegalMoves(bb, mvs);
//	notlegal.clear();
//
//	for( unsigned i=0; i<mvs.size(); ++i )
//	{
//		Bitboard bb_local(bb);
//		if(bb_local.MakeMove(mvs[i]))
//		{
//
//		}
//		else
//		{
//			notlegal.push_back(mvs[i]);
//		}
//	}
//	return notlegal;
//}

struct PerftHelper
{
	PerftHelper():
mates               (0), 
	captures            (0), 
	count               (0), 
	ep                  (0), 
	stalemates          (0), 
	promotions          (0), 
	castle_kingsides    (0), 
	castle_queensides   (0)
{}
ui64 mates, captures, count, ep, stalemates, promotions, castle_kingsides, castle_queensides;
};

ui64 Perft( Bitboard& bb, int depth, int current_depth, PerftHelper& ph )
{
	ui64 n_moves, i;
	ui64 nodes = 0;

	if (depth == 0) 
	{
		return 1;
	}

	GeneratePseudoLegalMoves(bb, bb.moves_arr[current_depth]);
	n_moves = bb.moves_arr[current_depth].size();

	for (i = 0; i < n_moves; i++) 
	{
		if(bb.MakeMove(bb.moves_arr[current_depth][i]))
		{
            bb.zobrists[current_depth+1] = bb.zobrists[current_depth];
            UpdateZobristFromMove(bb.zobrists[current_depth+1], bb.moves_arr[current_depth][i], bb);
            move& m = bb.moves_arr[current_depth][i];
            ui64 zobNow = ZobristFromBitboard(bb);
            ui64 zobUp =  bb.zobrists[current_depth+1];
            if(zobNow != zobUp)
            {
                Show<ShowTypes::Console>::Op(FenFromBitboard(bb));
                Show<ShowTypes::Console>::Op("-----");
            }
			nodes += Perft(bb,depth - 1, current_depth + 1, ph);
		}
		bb.UnmakeMove();
	}
	return nodes;
}
//std::vector< std::pair<move, int> > Divide( Bitboard& bb, int depth )
//{
//	std::vector<move> mvs;
//	std::vector< std::pair<move, int> > toReturn;
//	ui64 n_moves, i;
//	ui64 nodes = 0;
//
//	PerftHelper ph;
//	mvs = GeneratePseudoLegalMoves(bb, mvs);
//	n_moves = mvs.size();
//	for (i = 0; i < n_moves; i++) 
//	{
//		Bitboard bb_local(bb);
//		move m = mvs[i];
//		if(bb_local.MakeMove(m))
//		{
//			toReturn.push_back(std::make_pair(m,Perft(bb_local,depth - 1, 0, ph)));
//		}
//	}
//	return toReturn;
//}
#endif
