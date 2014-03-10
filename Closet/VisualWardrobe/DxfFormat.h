#pragma once

#include <string>
#include "mtxlib.h"
namespace DxfFormat{//输出DXF文件的函数
	template <typename _OutputStream> void PutLine( _OutputStream &os, const std::wstring &layer, float x0, float y0, float x1, float y1 )
	{//在文件中输出Line
		os<<L"\t0\nLINE\n\t8\n"<<layer<<L'\n'<<L"\t10\n"<<x0<<L'\n'<<L"\t20\n"<<y0<<L'\n'<<L"\t11\n"<<x1<<L'\n'<<L"\t21\n"<<y1<<L'\n';
	}
	template <typename _OutputStream, typename _PointConstIterator> void PutPolyLine( 
		_OutputStream &os, const std::wstring &layer, _PointConstIterator first, _PointConstIterator last,bool bClosed = true)
	{//在文件中输出PolyLine
		os<<L"\t0\nPOLYLINE\n\t8\n"<<layer<<L'\n'<<L"\t66\n1\n"<<L"\t70\n"<<int(bClosed)<<L'\n';
		for(_PointConstIterator i(first); i!=last; ++i){
			os<<L"\t0\nVERTEX\n\t8\n"<<layer<<L'\n'<<L"\t10\n"<<i->x<<L'\n'<<"\t20\n"<<i->y<<L'\n';
		}
		os<<L"\t0\nSEQEND\n\t8\n"<<layer<<L'\n';
	}
	template <typename _OutputStream> void PutRect( _OutputStream &os, const std::wstring &layer, float x0, float y0, float x1, float y1 )
	{//在文件中输出用PolyLine表示的矩形
		vector2 pts[4] = {vector2(x0,y0),vector2(x0,y1),vector2(x1,y1),vector2(x1,y0)};
		PutPolyLine(os,layer,&pts[0],&pts[4]);
	}
	template <typename _OutputStream> void PutDimension( _OutputStream &os, const std::wstring &layer, float x0, float y0, float x1, float y1 )
	{//在文件中输出尺寸标注
		os<<L"\t0\nDIMENSION\n\t8\n"<<layer<<L'\n'<<L"\t70\n0\n"
	}
};
