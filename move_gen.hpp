#ifndef __move_gen_hpp
#define __move_gen_hpp

#include "bitboard.hpp"
#include "bbfunctions.hpp"
#include "bitfunctions.hpp"
#include "piece_attacks.hpp"
#include "lookup.hpp"
#include "enums.hpp"
#include "move.hpp"

//#include "show.hpp"

#include <vector>



std::vector<move> GeneratePseudoLegalMoves( Bitboard& bb )
{
	std::vector<move> mvs;
	const bool isWhitesTurn = bb.IsWhitesTurn();
	ui64 pawns, bishops, rooks, queens, knights, king, opponent, self, opponentOrEp;
	ui64 empty = (bb.PiecesAt(PieceType::none));
	ui64 all = bb.PiecesAt(PieceType::all);
	PieceType::EPieceType epawns, ebishops, erooks, equeens, eknights, eking, eopponent, eself;
	const  Sq::ESq epSq = bb.EpSquare();
	const ui64 epSqBB = lookup::single_bit_set[epSq];
	/*const bool hasEp = epSq != Sq::none;*/
	
	move m;

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
				m.isEp = false;
				Sq::ESq sq = (Sq::ESq)BitScanForward(pawns);
				m.from = sq;
				const ui64 sqBB = lookup::single_bit_set[sq];
				pawns &= (~sqBB);
				ui64 normal = WhiteSinglePawnPushTargets(sqBB, empty);
				if(sq & Constants::not_rank_2)
					normal |= WhiteDoublePawnPushTargets(sqBB, empty);
				const ui64 norcnt    = PopulationCount(normal);
				for(unsigned j=0; j<norcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward(normal);
					normal &= (~lookup::single_bit_set[to]);
					m.to = to;
					m.isCapture = false;
					mvs.push_back(m);
				}
				ui64 captures = WhitePawnAnyAttacks (sqBB) &  opponentOrEp;
				const ui64 capcnt    = PopulationCount(captures);
				for(unsigned j=0; j<capcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward(captures);
					captures &= (~lookup::single_bit_set[to]);
					m.to = to;
					m.isCapture = true;
					m.captured = bb.PieceAtSq(to);
					m.isEp = (to == epSq);
					mvs.push_back(m);
				}
			}
		}
		else
		{
			for(unsigned i=0; i<pcnt; ++i)
			{
				m.isEp = false;
				Sq::ESq sq = (Sq::ESq)BitScanForward(pawns);
				m.from = sq;
				const ui64 sqBB = lookup::single_bit_set[sq];
				pawns &= (~sqBB);
				ui64 normal = BlackSinglePawnPushTargets(sqBB, empty);
				if(sq & Constants::not_rank_7)
					normal |= BlackDoublePawnPushTargets(sqBB, empty);
				const ui64 norcnt    = PopulationCount(normal);
				for(unsigned j=0; j<norcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward(normal);
					normal &= (~lookup::single_bit_set[to]);
					m.to = to;
					m.isCapture = false;
					mvs.push_back(m);
				}
				ui64 captures = BlackPawnAnyAttacks (sqBB) & opponentOrEp;
				const ui64 capcnt    = PopulationCount(captures);
				for(unsigned j=0; j<capcnt; ++j)
				{
					Sq::ESq to = (Sq::ESq)BitScanForward(captures);
					captures &= (~lookup::single_bit_set[to]);
					m.to = to;
					m.isEp = (to == epSq);
					m.isCapture = true;
					m.captured = bb.PieceAtSq(to);
					mvs.push_back(m);
				}
			}
		}
	}

	m.isEp = false;

	m.piece = ebishops;
	for(unsigned i=0; i<bcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward(bishops);
		m.from = sq;
		bishops &= (~lookup::single_bit_set[sq]);
		const ui64 pmvs = BishopAttacks(bb.PiecesAt(PieceType::all), sq);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(captures);
			captures &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = true;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(normal);
			normal &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = false;
			mvs.push_back(m);
		}
	}

	m.piece = erooks;
	for(unsigned i=0; i<rcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward(rooks);
		m.from = sq;
		rooks &= (~lookup::single_bit_set[sq]);
		const ui64 pmvs = RookAttacks(bb.PiecesAt(PieceType::all), sq);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(captures);
			captures &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = true;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(normal);
			normal &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = false;
			mvs.push_back(m);
		}
	}

	m.piece = equeens;
	for(unsigned i=0; i<qcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward(queens);
		m.from = sq;
		queens &= (~lookup::single_bit_set[sq]);
		const ui64 pmvs = QueenAttacks(bb.PiecesAt(PieceType::all), sq);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(captures);
			captures &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = true;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(normal);
			normal &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = false;
			mvs.push_back(m);
		}
	}

	m.piece = eknights;
	for(unsigned i=0; i<ncnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward(knights);
		m.from = sq;
		const ui64 sqBB = lookup::single_bit_set[sq];
		knights &= (~sqBB);
		const ui64 pmvs = KnightAttacks(sqBB);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty) ;
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(captures);
			captures &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = true;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(normal);
			normal &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = false;
			mvs.push_back(m);
		}
	}

	m.piece = eking;
	//for(unsigned i=0; i<kcnt; ++i)
	{
		Sq::ESq sq = (Sq::ESq)BitScanForward(king);
		m.from = sq;
		//king &= (~lookup::single_bit_set[sq]);
		const ui64 pmvs = KingAttacks(lookup::single_bit_set[sq]);
		ui64 captures  = pmvs & opponent;
		ui64 normal    = (pmvs & empty);
		const ui64 capcnt    = PopulationCount(captures);
		const ui64 norcnt    = PopulationCount(normal);
		for(unsigned j=0; j<capcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(captures);
			captures &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = true;
			m.captured = bb.PieceAtSq(to);
			mvs.push_back(m);
		}
		for(unsigned j=0; j<norcnt; ++j)
		{
			Sq::ESq to = (Sq::ESq)BitScanForward(normal);
			normal &= (~lookup::single_bit_set[to]);
			m.to = to;
			m.isCapture = false;
			mvs.push_back(m);
		}
	}
	return mvs;
}




#endif
