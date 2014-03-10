#pragma once
#include <cstdlib>
//���弸��������ά���ͷ��������ͣ�����������һЩ�ط�
template < typename V > struct VectorTraits;
template <> struct VectorTraits<vector2>{
	typedef float ScalarType;
	const static size_t Dimension = 2;
};
template <> struct VectorTraits<vector3>{
	typedef float ScalarType;
	const static size_t Dimension = 3;
};
template <> struct VectorTraits<vector4>{
	typedef float ScalarType;
	const static size_t Dimension = 4;
};
