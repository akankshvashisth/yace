
#ifndef __type_array_hpp
#define __type_array_hpp

#include <cassert>

//////////////////////////////////////////////

template< typename T, unsigned N >
struct type_array
{
    const T& back() const { return data[msize-1]; }
	        T& back()       { return data[msize-1]; }
  void push_back( const T& m )
	{  
		data[msize] = m; 
		++msize; 
		assert(msize < N);
	}
	const unsigned& size() const { return msize; }
	void pop_back()
	{
		--msize;
		assert(msize >= 0);
	}
	const T& operator[]( unsigned i ) const { assert(i<msize); return data[i]; }
	      T& operator[]( unsigned i )       { assert(i<msize); return data[i]; }
	void clear()
	{
		msize = 0;
	}
private:
    T data[N];
    unsigned int msize;
};

#endif

