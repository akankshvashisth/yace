#ifndef __tt_hpp
#define __tt_hpp

#include <vector>
#include "bitboard.hpp"
#include "move.hpp"

namespace BestMoveType
{
  enum EBestMoveType
  {
    exact,
    alpha,
    beta
  };
}

namespace TTProveReturnType
{
  enum ETTProveReturnType
  {
    value,
    best_move,
    miss
  };
}

struct TT_Entry
{
  move best_mv;
  ui64 zob;
  short eval;
  unsigned char depth;
  BestMoveType::EBestMoveType best_mv_type;
};

struct TT_Big_Entry
{
  ui64 zob;
  move_packed best_mv;
  //unsigned stuff;
  //unsigned char best_mv_type;
  //unsigned char depth;
  char eval[4];
  //char c[4];
};

struct TT
{
  void CreateTable( size_t kb )
  {
    size_t bytes = kb*1024;
    size = bytes/sizeof(TT_Entry);
    data.clear();
    data.resize(size);
  }
  
  void RecordHash(int depth, int val, BestMoveType::EBestMoveType type, Bitboard& b)
  {
    const ui64& zob = b.zobrists[b.zobrists.size()-1];
    TT_Entry& tt_entry = data[zob%size];
    tt_entry.best_mv = b.moves[b.moves.size()-1];
    tt_entry.best_mv_type = type;
    tt_entry.depth = depth;
    tt_entry.zob = zob;
  }

  TTProveReturnType::ETTProveReturnType ProbeHash(int depth, int alpha, int beta, Bitboard& b, int& val, move& mv)
  {
    const ui64& zob = b.zobrists[b.zobrists.size()-1];
    TT_Entry& tt_entry = data[zob%size];
    if(tt_entry.zob == zob)
    {
      if(tt_entry.depth >= depth)
      {
        switch(tt_entry.best_mv_type)
        {
        case BestMoveType::beta:
          if(tt_entry.eval >= beta)
          {
            val = tt_entry.eval;
            return TTProveReturnType::value;
          }
          break;
        case BestMoveType::alpha:
          if(tt_entry.eval <= alpha)
          {
            val = tt_entry.eval;
            return TTProveReturnType::value;
          }
          break;
        case BestMoveType::exact:
          {
            val = tt_entry.eval;
            return TTProveReturnType::value;
          }
          break;
        }
      }
      mv = tt_entry.best_mv;
      return TTProveReturnType::best_move;
    }
    return TTProveReturnType::miss;
  }
  

  std::vector<TT_Entry> data;
  size_t size;
};



#endif
