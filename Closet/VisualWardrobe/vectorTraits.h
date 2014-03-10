#pragma once
#include <cstdlib>
//定义几种向量的维数和分量的类型，用于其它的一些地方
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
