#pragma once

#include <opencv2/core/core.hpp>
#include "Basic.hpp"

namespace Basic{
	template<typename _T> struct FunctionTraits< cv::Mat_<_T> >{
		typedef _T ValueType;
		typedef int FirstArgType;
		typedef int SecondArgType;
	};
	
	template<typename _T>
	size_t SizeX(const cv::Mat_<_T> &m){
		return m.size().height;
	}

	template<typename _T>
	size_t SizeY(const cv::Mat_<_T> &m){
		return m.size().width;
	}
}