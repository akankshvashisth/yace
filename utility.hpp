#ifndef __utility_hpp
#define __utility_hpp

template<typename T>
inline const bool IsTrue( const T& bb ){ return bb!=0; }
template<typename T>
inline const bool IsFalse( const T& bb ){ return bb==0; }
template<typename T>
inline const bool If_IsTrue( const bool b, const T& bb )
{
  return b ? IsTrue(bb) : true;
}
template<typename T>
inline const bool If_IsFalse( const bool b, const T& bb )
{
  return b ? IsFalse(bb) : true;
}

#endif