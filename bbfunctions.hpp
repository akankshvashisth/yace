#ifndef __bbfunctions_hpp
#define __bbfunctions_hpp

#include "typedefs.hpp"
#include "constants.hpp"

inline BB S( BB b ){ return b<<8; }
inline BB N( BB b ){ return b>>8; }
inline BB E( BB b ){ return ( b << 1 ) & Constants::not_file_a; }
inline BB W( BB b ){ return ( b >> 1 ) & Constants::not_file_h; }

inline BB NE( BB b ){ return ( b << 9 ) & Constants::not_file_a; }
inline BB SE( BB b ){ return ( b >> 7 ) & Constants::not_file_a; }
inline BB SW( BB b ){ return ( b >> 9 ) & Constants::not_file_h; }
inline BB NW( BB b ){ return ( b << 7 ) & Constants::not_file_h; }

inline BB NNE(BB b) {return (b << 17) & Constants::not_file_a;}
inline BB NEE(BB b) {return (b << 10) & Constants::not_file_a_or_b;}
inline BB SEE(BB b) {return (b >>  6) & Constants::not_file_a_or_b;}
inline BB SSE(BB b) {return (b >> 15) & Constants::not_file_a ;}
inline BB NNW(BB b) {return (b << 15) & Constants::not_file_h ;}
inline BB NWW(BB b) {return (b <<  6) & Constants::not_file_g_or_h;}
inline BB SWW(BB b) {return (b >> 10) & Constants::not_file_g_or_h;}
inline BB SSW(BB b) {return (b >> 17) & Constants::not_file_h ;}
 
//inline BB NNE(BB b) {return (b & Constants::not_file_h ) << 17;}
//inline BB NEE(BB b) {return (b & Constants::not_file_g_or_h) << 10;}
//inline BB SEE(BB b) {return (b & Constants::not_file_g_or_h) >>  6;}
//inline BB SSE(BB b) {return (b & Constants::not_file_h ) >> 15;}
//inline BB NNW(BB b) {return (b & Constants::not_file_a ) << 15;}
//inline BB NWW(BB b) {return (b & Constants::not_file_a_or_b) <<  6;}
//inline BB SWW(BB b) {return (b & Constants::not_file_a_or_b) >> 10;}
//inline BB SSW(BB b) {return (b & Constants::not_file_a ) >> 17;}

////////////////////////////////////////////////////////////////////

inline BB WhiteSinglePawnPushTargets( BB p, BB empty ){ return N(p) & empty; }
inline BB BlackSinglePawnPushTargets( BB p, BB empty ){ return S(p) & empty; }
inline BB WhiteDoublePawnPushTargets( BB p, BB empty ){ return N(WhiteSinglePawnPushTargets(p,empty)) & empty & Constants::rank_4; }
inline BB BlackDoublePawnPushTargets( BB p, BB empty ){ return S(BlackSinglePawnPushTargets(p,empty)) & empty & Constants::rank_5; }
inline BB WhitePawnsAbleToPush(BB p, BB empty) { return S(empty) & p; }
inline BB WhitePawnsAbleToDoublePush(BB p, BB empty) { return WhitePawnsAbleToPush(p, (S(empty & Constants::rank_4) & empty)); }
inline BB BlackPawnsAbleToPush(BB p, BB empty) { return N(empty) & p; }
inline BB BlackPawnsAbleToDoublePush(BB p, BB empty) { return BlackPawnsAbleToPush(p, (N(empty & Constants::rank_5) & empty)); }

inline BB WhitePawnEastAttacks(BB p) { return NE(p); }
inline BB WhitePawnWestAttacks(BB p) { return NW(p); }
inline BB BlackPawnEastAttacks(BB p) { return SE(p); }
inline BB BlackPawnWestAttacks(BB p) { return SW(p); }
inline BB WhitePawnAnyAttacks(BB p)    { return WhitePawnEastAttacks(p) | WhitePawnWestAttacks(p); }
inline BB WhitePawnDoubleAttacks(BB p) { return WhitePawnEastAttacks(p) & WhitePawnWestAttacks(p); }
inline BB WhitePawnSingleAttacks(BB p) { return WhitePawnEastAttacks(p) ^ WhitePawnWestAttacks(p); }
inline BB BlackPawnAnyAttacks(BB p)    { return BlackPawnEastAttacks(p) | BlackPawnWestAttacks(p); }
inline BB BlackPawnDoubleAttacks(BB p) { return BlackPawnEastAttacks(p) & BlackPawnWestAttacks(p); }
inline BB BlackPawnSingleAttacks(BB p) { return BlackPawnEastAttacks(p) ^ BlackPawnWestAttacks(p); }

inline BB WhitePawnsAbleToCaptureEast(BB wpawns, BB bpieces) { return wpawns & BlackPawnWestAttacks(bpieces); }
inline BB WhitePawnsAbleToCaptureWest(BB wpawns, BB bpieces) { return wpawns & BlackPawnEastAttacks(bpieces); }
inline BB WhitePawnsAbleToCaptureAny (BB wpawns, BB bpieces) { return wpawns & BlackPawnAnyAttacks(bpieces); }
inline BB BlackPawnsAbleToCaptureEast(BB bpawns, BB wpieces) { return bpawns & WhitePawnWestAttacks(wpieces); }
inline BB BlackPawnsAbleToCaptureWest(BB bpawns, BB wpieces) { return bpawns & WhitePawnEastAttacks(wpieces); }
inline BB BlackPawnsAbleToCaptureAny (BB bpawns, BB wpieces) { return bpawns & WhitePawnAnyAttacks(wpieces); }

inline BB WhiteSafePawnSquares(BB wpawns, BB bpawns) 
{
   const BB wPawnEastAttacks =  WhitePawnEastAttacks (wpawns);
   const BB wPawnWestAttacks =  WhitePawnWestAttacks (wpawns);
   const BB bPawnEastAttacks =  BlackPawnEastAttacks (bpawns);
   const BB bPawnWestAttacks =  BlackPawnWestAttacks (bpawns);
   const BB wPawnDblAttacks  =  wPawnEastAttacks &  wPawnWestAttacks;
   const BB wPawnOddAttacks  =  wPawnEastAttacks ^  wPawnWestAttacks;
   const BB bPawnDblAttacks  =  bPawnEastAttacks &  bPawnWestAttacks;
   const BB bPawnAnyAttacks  =  bPawnEastAttacks |  bPawnWestAttacks;
   return wPawnDblAttacks |~bPawnAnyAttacks | (wPawnOddAttacks &~bPawnDblAttacks);
}

inline BB BlackSafePawnSquares(BB wpawns, BB bpawns) 
{
   const BB wPawnEastAttacks =  WhitePawnEastAttacks (wpawns);
   const BB wPawnWestAttacks =  WhitePawnWestAttacks (wpawns);
   const BB bPawnEastAttacks =  BlackPawnEastAttacks (bpawns);
   const BB bPawnWestAttacks =  BlackPawnWestAttacks (bpawns);
   const BB bPawnDblAttacks  =  bPawnEastAttacks &  bPawnWestAttacks;
   const BB bPawnOddAttacks  =  bPawnEastAttacks ^  bPawnWestAttacks;
   const BB wPawnDblAttacks  =  wPawnEastAttacks &  wPawnWestAttacks;
   const BB wPawnAnyAttacks  =  wPawnEastAttacks |  wPawnWestAttacks;
   return bPawnDblAttacks |~wPawnAnyAttacks | (bPawnOddAttacks &~wPawnDblAttacks);
}

inline BB WhitePawnRam(BB wpawns, BB bpawns) {return S(bpawns) & wpawns;}
inline BB BlackPawnRam(BB wpawns, BB bpawns) {return N(wpawns) & bpawns;}

inline BB WhiteEastPawnLever(BB wpawns, BB bpawns) { return wpawns & BlackPawnWestAttacks(bpawns); }
inline BB WhiteWestPawnLever(BB wpawns, BB bpawns) { return wpawns & BlackPawnEastAttacks(bpawns); }
inline BB WhiteAnyPawnLever (BB wpawns, BB bpawns) { return WhiteEastPawnLever(wpawns, bpawns) | WhiteWestPawnLever(wpawns, bpawns); }
inline BB BlackEastPawnLever(BB wpawns, BB bpawns) { return bpawns & WhitePawnWestAttacks(wpawns); }
inline BB BlackWestPawnLever(BB wpawns, BB bpawns) { return bpawns & WhitePawnEastAttacks(wpawns); }
inline BB BlackAnyPawnLever (BB wpawns, BB bpawns) { return BlackEastPawnLever(wpawns, bpawns) | BlackWestPawnLever(wpawns, bpawns); }
inline BB BlackEastPawnLever(BB whiteWestPawnLever) { return whiteWestPawnLever << 7; }
inline BB BlackWestPawnLever(BB whiteEastPawnLever) { return whiteEastPawnLever << 9; }

inline BB WhiteInnerPawnLever(BB wpawns, BB bpawns) 
{
   return ( WhiteEastPawnLever(wpawns, bpawns) & Constants::file_abc ) | ( WhiteWestPawnLever(wpawns, bpawns) & Constants::file_fgh );
}
 
inline BB WhiteOuterPawnLever(BB wpawns, BB bpawns) 
{
   return ( WhiteEastPawnLever(wpawns, bpawns) & Constants::file_efg ) | ( WhiteWestPawnLever(wpawns, bpawns) & Constants::file_bcd );
}
 
inline BB WhiteCenterPawnLever(BB wpawns, BB bpawns) 
{
   return ( WhiteEastPawnLever(wpawns, bpawns) & Constants::file_d ) | ( WhiteWestPawnLever(wpawns, bpawns) & Constants::file_e );
}

inline BB BlackInnerPawnLever(BB wpawns, BB bpawns) 
{
  return ( BlackEastPawnLever(wpawns, bpawns) & Constants::file_abc ) | ( BlackWestPawnLever(wpawns, bpawns) & Constants::file_fgh );
}
 
inline BB BlackOuterPawnLever(BB wpawns, BB bpawns) 
{
   return ( BlackEastPawnLever(wpawns, bpawns) & Constants::file_efg ) | ( BlackWestPawnLever(wpawns, bpawns) & Constants::file_bcd );
}
 
inline BB BlackCenterPawnLever(BB wpawns, BB bpawns) 
{
   return ( BlackEastPawnLever(wpawns, bpawns) & Constants::file_d ) | ( BlackWestPawnLever(wpawns, bpawns) & Constants::file_e );
}


inline BB KingAttacks( BB k )
{ 
  //return N(k)|E(k)|W(k)|S(k)|NE(k)|NW(k)|SE(k)|SW(k); 
   ui64 attacks = E(k)|W(k);
   k|=attacks;
   attacks |= N(k)|S(k);
   return attacks;
}

inline BB KnightAttacks(BB n) 
{
   static const ui64 c1(0x7f7f7f7f7f7f7f7f);
   static const ui64 c2(0x3f3f3f3f3f3f3f3f);
   static const ui64 c3(0xfefefefefefefefe);
   static const ui64 c4(0xfcfcfcfcfcfcfcfc);

   const BB l1 = (n >> 1) & c1;
   const BB l2 = (n >> 2) & c2;
   const BB r1 = (n << 1) & c3;
   const BB r2 = (n << 2) & c4;
   const BB h1 = l1 | r1;
   const BB h2 = l2 | r2;
   return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

inline BB KnightFill(BB knights) { return KnightAttacks(knights) | knights; }
inline BB KingFill(BB kings){ return KingAttacks(kings) | kings; }

inline BB ForkTargetSquare(BB targets) 
{
   BB west, east, attack, forks;
   east		= E(targets);
   west		= W(targets);
   attack	=  east << 16;
   forks	= (west << 16) & attack;
   attack	|=  west << 16;
   forks	|= (east >> 16) & attack;
   attack	|=  east >> 16;
   forks	|= (west >> 16) & attack;
   attack	|=  west >> 16;
   east		= E(east);
   west		= W(west);
   forks	|= (east <<  8) & attack;
   attack	|=  east <<  8;
   forks	|= (west <<  8) & attack;
   attack	|=  west <<  8;
   forks	|= (east >>  8) & attack;
   attack	|=  east >>  8;
   forks	|= (west >>  8) & attack;
   return forks;
}
   

#endif