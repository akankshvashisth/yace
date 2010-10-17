#ifndef __bitfunctions_hpp
#define __bitfunctions_hpp

#include "defines.hpp"
#include "typedefs.hpp"
#include "bithacks.hpp"

bool IsPopulationCount_1( const ui64 x ){ return ( x != 0 && (x & (x-1)) == 0 ); }
bool IsPopulationCount_LE_1( const ui64 x ){ return ((x & (x-1)) == 0); }
bool IsPopulationCount_G_1( const ui64 x ){ return ( x & (x-1) ) != 0; }


inline ui64 RotL(ui64 x, int s){ return rotateLeft(x,s); }
inline ui64 RotR(ui64 x, int s){ return rotateRight(x,s); }


inline ui64 GenShift(ui64 x, int s){ return genShiftBranchless(x,s); }

inline ui64 SingleBitSet( ui64 n ){ return ui64(1)<<n; } // or lookup[n];

inline void SetBit( ui64& x, ui64 n ){ x |= SingleBitSet(n); }
inline bool IsBitSet( ui64 x, ui64 n ){ return IS_TRUE( x & SingleBitSet(n) ); }
inline void ClearBit( ui64& x, ui64 n ){ x &= ~SingleBitSet(n); } //try [or, xor] rather than [not, and]
inline void ToggleBit( ui64& x, ui64 n ){ x ^= SingleBitSet(n); }

inline unsigned PopulationCount (ui64 x){ return popCount(x); }
inline unsigned HammingDistance (ui64 a, ui64 b) {return PopulationCount( a ^ b);}

inline unsigned BitScanForward(ui64 x){ return bitScanForward_DeBruijn(x); }
inline unsigned BitScanReverse(ui64 x){ return bitScanReverse_01(x); }

#endif