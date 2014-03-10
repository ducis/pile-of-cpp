#pragma once

#include "StackOnly.hpp"
#include <algorithm>
#include <iosfwd>

namespace Utility{
	//template<class _OutputStream>
	//bool StreamNeedsDelimiter(const _OutputStream &os);

	//template<>
	//bool StreamNeedsDelimiter(const std::ios_base &os){
	//	return os.
	//}

	//template<class _InputIterator, class _OutputStream, class _Delimiter>
	//_OutputStream &AutoDelimitedStreamOutput( _InputIterator first, _InputIterator last, _OutputStream &os, const _Delimiter &delimiter){
	//	if( StreamNeedsDelimiter(os) ){
	//		for( _InputIterator itr( first ); itr!= last; ++itr){
	//			os<<*itr<<delimiter;
	//		}
	//	}else{
	//		std::copy(first,last,std::ostream_iterator(os));
	//	}
	//}
}
