#pragma once
#include <istream>
#include "3d.h"


//给向量和C-style数组重载stream的输入( >> )运算符, 都是逐个读取所有元素


inline std::wistream &operator>>( std::wistream &is,vector3 &v){
	is>>v.x;
	is>>v.y;
	is>>v.z;
	return is;
}
inline std::wistream &operator>>( std::wistream &is,vector2 &v){
	is>>v.x;
	is>>v.y;
	return is;
}
inline std::wistream &operator>>( std::wistream &is,AABB &aabb ){
	return is>>aabb.first>>aabb.second;
}
#include <vector>
#include <iterator>

template<typename _Ty,typename _stream_type>
_stream_type &operator>>( _stream_type &is, std::vector<_Ty> &v ){
	istream_iterator<_Ty,_stream_type::char_type> ivr( is );
	v.assign( ivr, istream_iterator<_Ty,wchar_t>() );
	return is;
}

template<typename _Ty,size_t _sz,typename _stream_type>
_stream_type &operator>>( _stream_type &is, _Ty (&a)[_sz] ){
	for(size_t i = 0; i<_sz; ++i){
		is>>a[i];
	}
	return is;
}

//template std::wistream &operator>><int>( std::wistream &is, std::vector<int> &v );
//
//std::wistream &operator>>( std::wistream &is, std::vector<int> &v ){
//	v.clear();
//	while(true){
//		int x;
//		is>>x;
//		if( is.gcount()>0 ){
//			v.push_back(x);
//		}else{
//			break;
//		}
//	}
//	return is;
//}
