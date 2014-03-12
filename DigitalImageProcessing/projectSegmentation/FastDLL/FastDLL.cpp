#include "MeanShift.h"
#include "FastDLL.h"
#include <algorithm>
#include <fstream>


using namespace MeanShift;

extern "C" {

	__declspec(dllexport) void MeanShiftDPSrgbf(
		float *out,
		unsigned long nRowImg, unsigned long nColImg, const float *img,
		float spatialBandwidth, float colorBandwidth, unsigned long maxIteration, float convergenceDistSqr
		)
	{
		const Matrix< FixedVector<float,3> > in(nRowImg,nColImg,(FixedVector<float,3>*)img);
		const Matrix< FixedVector<float,3> > m( 
			DiscontinuityPreservingSmooth( in,
			spatialBandwidth, colorBandwidth, maxIteration, convergenceDistSqr) );
		std::copy(m.Begin(),m.End(),(FixedVector<float,3>*)out);
		//std::copy(in.Begin(),in.End(),(FixedVector<float,3>*)out);
	}

	
	__declspec(dllexport) void PrintRGBf(unsigned long nRowImg, unsigned long nColImg, const float *img)
	{
		std::wofstream(L"checkpoint.txt")<<L"checkpoint";
		const Matrix< FixedVector<float,3> > m(nRowImg,nColImg,(FixedVector<float,3>*)img);
		std::wofstream(L"print.txt")<<m;
	}


	__declspec(dllexport) void MeanShiftSegmentationrgbf(
		unsigned long *out,
		unsigned long nRowImg, unsigned long nColImg, const float *img,
		float spatialBandwidth, float colorBandwidth, unsigned long minRegionSize, unsigned long maxIteration, float convergenceDistSqr
		)
	{
		const Matrix< FixedVector<float,3> > in(nRowImg,nColImg,(FixedVector<float,3>*)img);
		const Matrix< unsigned long > m( 
			Segment( in,
			spatialBandwidth, colorBandwidth, minRegionSize, maxIteration, convergenceDistSqr) );
		std::copy(m.Begin(),m.End(),out);
	}

	
	__declspec(dllexport) void PruneSmallRegionsul(
		unsigned long *out,
		unsigned long nRowImg, unsigned long nColImg, const unsigned long *idMap,
		unsigned long minRegionSize
		)
	{
		Matrix< unsigned long > m(nRowImg,nColImg,idMap);
		RepaintSmallRegions_Inplace(m,minRegionSize,0xFFFFFFFFUL);
		PruneRegionsById_Inplace(m,0xFFFFFFFFUL);
		std::copy(m.Begin(),m.End(),out);
	}


	__declspec(dllexport) void MeanShiftEdgenessMaprgbf(
		float *out,
		unsigned long nRowImg,unsigned long nColImg, const float *img,
		float spatialBandwidth,float colorBandwidth, unsigned long maxIteration, float convergenceDistSqr
		)
	{
		const Matrix< FixedVector<float,3> > in(nRowImg,nColImg,(FixedVector<float,3>*)img);
		const Matrix< float > m( 
			EdgenessMap( in, spatialBandwidth, colorBandwidth, maxIteration, convergenceDistSqr) );
		std::copy(m.Begin(),m.End(),out);
	}

}
