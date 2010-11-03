#ifndef __tt_hpp
#define __tt_hpp

#include <vector>
#include "bitboard.hpp"
#include "move.hpp"
//#include "show.hpp"

namespace BestMoveType
{
  enum EBestMoveType
  {
    exact,
    alpha,
    beta,
    none
  };
}

namespace TTProveReturnType
{
  enum ETTProveReturnType
  {
    value,
    best_move,
    miss,
    none
  };
}

struct TT_E
{
  TT_E():zob(0), best_mv(), eval(0), depth(0), best_mv_type(unsigned char(BestMoveType::none)){}
  ui64 zob;
  move_packed best_mv;
  short eval;
  unsigned char depth;
  unsigned char best_mv_type;
};

struct TT_Entry
{
  move best_mv;
  ui64 zob;
  int eval;
  int depth;
  BestMoveType::EBestMoveType best_mv_type;
};

//struct TT_Big_Entry
//{
//  ui64 zob;
//  move_packed best_mv;
//  //unsigned stuff;
//  //unsigned char best_mv_type;
//  //unsigned char depth;
//  char eval[4];
//  //char c[4];
//};

struct TT
{
  void Clear()
  {
    data.clear();
    data.resize(size);
  }

  void CreateTable( size_t kb )
  {
    size_t bytes = kb*1024;
    size = bytes/sizeof(TT_E);
    Clear();
  }
  
  void RecordHash(int depth, int val, BestMoveType::EBestMoveType type, Bitboard& b)
  {
    //Show<ShowTypes::Console>::Op(val);
    //Show<ShowTypes::Console>::Op("rec");
    size_t sz = b.moves.size();
    ui64 zob;

    if(sz!=0)
    {
      zob = b.zobrists[b.zobrists.size()-2];
    }
    else
    {
      zob = b.zobrists[0];
    }

    TT_E& tt_entry = data[zob%size];

    if(sz!=0)
    {
      tt_entry.best_mv = /*move_packed*/(b.moves.back());
    }

    tt_entry.best_mv_type = (type);
    tt_entry.eval = val;
    tt_entry.depth = (depth);
    tt_entry.zob = zob;
  }

  TTProveReturnType::ETTProveReturnType ProbeHash(int depth, int alpha, int beta, Bitboard& b, int& val, move& mv)
  {
    const ui64& zob = b.zobrists.back();
    TT_E& tt_entry = data[zob%size];
    if(tt_entry.zob == zob)
    {
      if((int)(tt_entry.depth) >= depth)
      {
        switch(tt_entry.best_mv_type)
        {
        case BestMoveType::beta:
          if(tt_entry.eval >= beta)
          {
            val = (int)(tt_entry.eval);
            return TTProveReturnType::value;
          }
          break;
        case BestMoveType::alpha:
          if(tt_entry.eval <= alpha)
          {
            val = (int)(tt_entry.eval);
            return TTProveReturnType::value;
          }
          break;
        case BestMoveType::exact:
          {
            val = (int)(tt_entry.eval);
            return TTProveReturnType::value;
          }
          break;
        }
      }
      mv = MoveFromMovePacked(tt_entry.best_mv);
      return TTProveReturnType::best_move;
    }
    return TTProveReturnType::miss;
  }
  
  std::vector<TT_E> data;
  size_t size;
};



#endif
