#ifndef __bithacks_hpp
#define __bithacks_hpp

#include "typedefs.hpp"
#include "defines.hpp"

//////////////////////////////////////////////////////////////////////////////
//#pragma warning( push )
//#pragma warning( disable : 102 ) //compiler will add 4000

//http://williamchan.ca/portfolio/assembly/ssememcpy/source/
//void X_aligned_memcpy_sse2(void* dest, const void* src, const unsigned long size_t)
//{
//  //__asm
//  asm
//  {
//    mov esi, src;    //src pointer
//    mov edi, dest;   //dest pointer
//
//    mov ebx, size_t; //ebx is our counter
//    shr ebx, 7;      //divide by 128 (8 * 128bit registers)
//
//loop_copy:
//    prefetchnta 128[ESI]; //SSE2 prefetch
//    prefetchnta 160[ESI];
//    prefetchnta 192[ESI];
//    prefetchnta 224[ESI];
//
//    movdqa xmm0, 0[ESI]; //move data from src to registers
//    movdqa xmm1, 16[ESI];
//    movdqa xmm2, 32[ESI];
//    movdqa xmm3, 48[ESI];
//    movdqa xmm4, 64[ESI];
//    movdqa xmm5, 80[ESI];
//    movdqa xmm6, 96[ESI];
//    movdqa xmm7, 112[ESI];
//
//    movntdq 0[EDI], xmm0; //move data from registers to dest
//    movntdq 16[EDI], xmm1;
//    movntdq 32[EDI], xmm2;
//    movntdq 48[EDI], xmm3;
//    movntdq 64[EDI], xmm4;
//    movntdq 80[EDI], xmm5;
//    movntdq 96[EDI], xmm6;
//    movntdq 112[EDI], xmm7;
//
//    add esi, 128;
//    add edi, 128;
//    dec ebx;
//
//    jnz loop_copy; //loop please
//loop_copy_end:
//  }
//}

//#pragma warning( pop )


///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
unsigned popCount (ui64 x) //Pretty close to best for sparse! possibly very useful when endgame begins.
{
  unsigned count = 0;
  while (x)
  {
    count++;
    x &= x - 1; // reset LS1B
  }
  return count;
}

///////////////////////////////////////////////////////////////
unsigned char popCountOf256Bytes[256];
unsigned short popCountOfShorts65536[65536];

void initpopCountOfByte256()
{
  popCountOf256Bytes[0] = 0;
  for (int i = 1; i < 256; i++)
    popCountOf256Bytes[i] = (i & 1) + popCountOf256Bytes[i / 2];
}

void initpopCountOfShorts65536()
{
  popCountOfShorts65536[0] = 0;
  for (int i = 1; i < 65536; i++)
    popCountOfShorts65536[i] = (i & 1) + popCountOfShorts65536[i / 2];
}

unsigned popCount2 (ui64 x)
{
  return popCountOf256Bytes[ x        & 0xff] +
    popCountOf256Bytes[(x >>  8) & 0xff] +
    popCountOf256Bytes[(x >> 16) & 0xff] +
    popCountOf256Bytes[(x >> 24) & 0xff] +
    popCountOf256Bytes[(x >> 32) & 0xff] +
    popCountOf256Bytes[(x >> 40) & 0xff] +
    popCountOf256Bytes[(x >> 48) & 0xff] +
    popCountOf256Bytes[ x >> 56];
}

unsigned popCount3 (ui64 x)
{
  return popCountOfShorts65536[ x & 0xffff] +
    popCountOfShorts65536[(x >> 16) & 0xffff] +
    popCountOfShorts65536[(x >> 32) & 0xffff] +
    popCountOfShorts65536[(x >> 48)];
}

unsigned popCount4 (ui64 x) //better than the above one
{
  unsigned char * p = (unsigned char *) &x;
  return popCountOf256Bytes[p[0]] +
    popCountOf256Bytes[p[1]] +
    popCountOf256Bytes[p[2]] +
    popCountOf256Bytes[p[3]] +
    popCountOf256Bytes[p[4]] +
    popCountOf256Bytes[p[5]] +
    popCountOf256Bytes[p[6]] +
    popCountOf256Bytes[p[7]];
}

unsigned popCount5 (ui64 x) //better than the above one
{
  unsigned short * p = (unsigned short *) &x;
  return popCountOfShorts65536[p[0]] +
    popCountOfShorts65536[p[1]] +
    popCountOfShorts65536[p[2]] +
    popCountOfShorts65536[p[3]];
}

unsigned popCount6 (ui64 x)
{
  static const ui64 k1 = C64(0x5555555555555555ULL);
  static const ui64 k2 = C64(0x3333333333333333ULL);
  static const ui64 k4 = C64(0x0f0f0f0f0f0f0f0fULL);
  static const ui64 kf = C64(0x0101010101010101ULL);
  x =  x       - ((x >> 1)  & k1); //put count of each 2 bits into those 2 bits
  x = (x & k2) + ((x >> 2)  & k2); //put count of each 4 bits into those 4 bits
  x = (x       +  (x >> 4)) & k4 ; //put count of each 8 bits into those 8 bits
  x = (x * kf) >> 56; //returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...
  return (unsigned) x;
}

unsigned slowmul_popCount (ui64 x)
{
  static const ui64 k1 = C64(0x5555555555555555ULL);
  static const ui64 k2 = C64(0x3333333333333333ULL);
  static const ui64 k4 = C64(0x0f0f0f0f0f0f0f0fULL);
  x =  x       - ((x >> 1)  & k1); //put count of each 2 bits into those 2 bits
  x = (x & k2) + ((x >> 2)  & k2); //put count of each 4 bits into those 4 bits
  x = (x       +  (x >> 4)) & k4 ; //put count of each 8 bits into those 8 bits
  x += x >>  8;  //put count of each 16 bits into their lowest 8 bits
  x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
  x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
  return ((unsigned)(x))&0xFF;
}

//////////////////////////////////////////////////////////////////

// return popCount(x) + popCount(y) + popCount(z)
int popCount7 (ui64 x, ui64 y, ui64 z) {
  static const ui64 k1 = C64(0x5555555555555555ULL);
  static const ui64 k2 = C64(0x3333333333333333ULL);
  static const ui64 k4 = C64(0x0f0f0f0f0f0f0f0fULL);
  static const ui64 kf = C64(0x0101010101010101ULL);
  ui64 maj = ((x ^ y ) & z) | (x & y);
  ui64 odd = ((x ^ y ) ^ z);
  maj =  maj - ((maj >> 1) & k1 );
  odd =  odd - ((odd >> 1) & k1 );
  maj = (maj & k2) + ((maj >> 2) & k2);
  odd = (odd & k2) + ((odd >> 2) & k2);
  maj = (maj + (maj >> 4)) & k4;
  odd = (odd + (odd >> 4)) & k4;
  odd = ((maj + maj + odd) * kf ) >> 56;
  return (int) odd;
}

//////////////////////////////////////////////////////////////////

ui64 odd(ui64 x, ui64 y, ui64 z) {return x^y^z;}
ui64 maj(ui64 x, ui64 y, ui64 z) {return ((x^y)&z)|(x&y);}

void attackCounts(ui64 t[3], const ui64 s[7]) {
  const ui64 one1 = odd(s[0], s[1], s[2]);
  const ui64 two1 = maj(s[0], s[1], s[2]);
  const ui64 one2 = odd(s[3], s[4], s[5]);
  const ui64 two2 = maj(s[3], s[4], s[5]);
  t[0] = odd(s[6], one1, one2);
  const ui64 two3 = maj(s[6], one1, one2);
  t[1] = odd(two1, two2, two3);
  t[2] = maj(two1, two2, two3);
}

//exactly7attacks :=   t[2] &  t[1] &  t[0]
//exactly6attacks :=   t[2] &  t[1] & ~t[0]
//exactly5attacks :=   t[2] & ~t[1] &  t[0]
//exactly4attacks :=   t[2] & ~t[1] & ~t[0]
//exactly3attacks :=  ~t[2] &  t[1] &  t[0]
//exactly2attacks :=  ~t[2] &  t[1] & ~t[0]
//exactly1attack  :=  ~t[2] & ~t[1] &  t[0]
//noAttack        :=  ~t[2] & ~t[1] & ~t[0]
//
//atLeast4attacks :=                   t[2]
//atLeast2attacks := atLeast4attacks | t[1]
//atLeast1attack  := atLeast2attacks | t[0]
//noAttack        := ~atLeast1attack
//exactly1attack  :=  atLeast1attack  ^ atLeast2attacks

////////////////////////////////////////////////////////////////////

inline ui64 rotateLeft (ui64 x, int s) { assert(false); return 0;} //_rotl64(x, s);}
inline ui64 rotateRight(ui64 x, int s) { assert(false); return 0;}//_rotr64(x, s);}

inline ui64 rotateLeftWithShift (ui64 x, int s) {return (x << s) | (x >> (64-s));}
inline ui64 rotateRightWithShift(ui64 x, int s) {return (x >> s) | (x << (64-s));}

///////////////////////////////////////////////////////////////////



inline ui64 genShiftBranch(ui64 x, int s) {return (s > 0) ? (x << s) : (x >> -s);}
inline ui64 genShiftBranchless(ui64 x, int s)
{
   char left  =   (char) s;
   char right = -((char)(s >> 8) & left);
   return (x >> right) << (right + left);
}

//inline ui64 genShiftASM(ui64 x, int s)
//{
//  __asm
//  {
//     ; input
//     ; ecx - shift amount,
//     ;       left if positive
//     ;       right if negative
//     ; rax - bitboard to shift
//     mov   rax, x;
//     mov   ecx, s;
//     mov   dl,  cl
//     and   cl,  ch
//     neg   cl
//     shr   rax, cl
//     add   cl,  dl
//     shl   rax, cl
//  }
//}

const int index64_DeBruijn[64] =
{
   63,  0, 58,  1, 59, 47, 53,  2,
   60, 39, 48, 27, 54, 33, 42,  3,
   61, 51, 37, 40, 49, 18, 28, 20,
   55, 30, 34, 11, 43, 14, 22,  4,
   62, 57, 46, 52, 38, 26, 32, 41,
   50, 36, 17, 19, 29, 10, 13, 21,
   56, 45, 25, 31, 35, 16,  9, 12,
   44, 24, 15,  8, 23,  7,  6,  5
};

/**
 * bitScanForward
 * @author Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 146 ) //compiler will add 4000
#endif
unsigned bitScanForward_DeBruijn(ui64 bb)
{
   static const ui64 debruijn64(0x07EDD5E59A4E28C2ULL);
   assert (bb != 0);
   return index64_DeBruijn[((bb & -bb) * debruijn64) >> 58];
}

#if defined(_MSC_VER)
#pragma warning( pop )
#endif



#endif
