#pragma once

#include <cstdlib>
#include <algorithm>
#include <utility>
#include "Basic.hpp"

namespace LucasKanade{
	using namespace Basic;


	template< typename _Image, typename _WeightFunction, typename _WindowRadius, typename _MaxIteration>//, typename _Position>
	std::pair< typename FunctionTraits<_Image>::ValueType, typename FunctionTraits<_Image>::ValueType > IterativeOpticalFlowAtPoint(
		const _Image &current,
		const _Image &next,
		const _WeightFunction &weightFunc,
		const _WindowRadius &windowRad,
		const _MaxIteration &maxIteration,
		//const _Position &x,
		//const _Position &y
		size_t x,
		size_t y
		)
	{
		//in MATLAB:
		//>> [ a b; b c ]\[-u;-v]
		// 
		//ans =
		//
		//  (b*v - c*u)/(a*c - b^2)
		// -(a*v - b*u)/(a*c - b^2)
		typedef typename FunctionTraits<_Image>::ValueType scalar_type;
		scalar_type a(0),b(0),c(0);
		size_t xr( std::min(x+windowRad+1,SizeX(current)-1) );
		size_t yr( std::min(y+windowRad+1,SizeY(current)-1) );
		size_t xl( x<=windowRad? 1 : x-windowRad );
		size_t yl( y<=windowRad? 1 : y-windowRad );

		for(size_t xx(xl); xx<xr; ++xx){
			for(size_t yy(yl); yy<yr; ++yy){
				scalar_type ddx( current(xx+1,yy) - current(xx-1,yy) );
				scalar_type ddy( current(xx,yy+1) - current(xx,yy-1) );
				typename _WeightFunction::result_type w( weightFunc(xx-x,yy-y) );
				a += w*ddx*ddx;
				b += w*ddx*ddy;
				c += w*ddy*ddy;
			}
		}
		scalar_type denominator( a*c - b*b );

		if(0==denominator){
			return std::pair<scalar_type,scalar_type>(0,0);
		}
		std::pair<scalar_type,scalar_type> vel(0,0);
		for(size_t i(maxIteration);i;--i){
			int deltaX(vel.first);
			int deltaY(vel.second);
			if(xl+deltaX<1||yl+deltaY<1||xr+deltaX>SizeX(current)-1||yr+deltaY>SizeY(current)-1){
				break;
			}
			scalar_type u(0),v(0);
			for(size_t xx(xl); xx<xr; ++xx){
				for(size_t yy(yl); yy<yr; ++yy){
					scalar_type ddx( current(xx+1,yy) - current(xx-1,yy) );
					scalar_type ddy( current(xx,yy+1) - current(xx,yy-1) );
					scalar_type ddt( next(xx+deltaX,yy+deltaY) - current(xx,yy) );
					typename _WeightFunction::result_type w( weightFunc(xx-x,yy-y) );
					u += w*ddx*ddt;
					v += w*ddy*ddt;
				}
			}
			vel.first+=( b*v - c*u )/denominator;
			vel.second+=( b*u - a*v )/denominator;
		}
		return vel;
	}



	template<typename _Image, typename _WeightFunction, typename _WindowRadius>
	std::pair< typename FunctionTraits<_Image>::ValueType, typename FunctionTraits<_Image>::ValueType > OpticalFlowAtPoint(
		const _Image &current,
		const _Image &next,
		const _WeightFunction &weightFunc,
		const _WindowRadius &windowRad,
		size_t x,
		size_t y
		)
	{
		//in MATLAB:
		//>> [ a b; b c ]\[-u;-v]
		// 
		//ans =
		// 
		//  (b*v - c*u)/(a*c - b^2)
		// -(a*v - b*u)/(a*c - b^2)
		typedef typename FunctionTraits<_Image>::ValueType scalar_type;
		scalar_type a(0),b(0),c(0),u(0),v(0);
		size_t xr( std::min(x+windowRad+1,SizeX(current)-1) );
		size_t yr( std::min(y+windowRad+1,SizeX(current)-1) );
		for(size_t xx(x<=windowRad? 1 : x-windowRad); xx<xr; ++xx){
			for(size_t yy(y<=windowRad? 1 : y-windowRad); yy<yr; ++yy){
				scalar_type ddx( current(xx+1,yy) - current(xx-1,yy) );
				scalar_type ddy( current(xx,yy+1) - current(xx,yy-1) );
				scalar_type ddt( next(xx,yy) - current(xx,yy) );
				typename _WeightFunction::result_type w( weightFunc(xx-x,yy-y) );
				a += w*ddx*ddx;
				b += w*ddx*ddy;
				c += w*ddy*ddy;
				u += w*ddx*ddt;
				v += w*ddy*ddt;
			}
		}
		scalar_type denominator( a*c - b*b );
		if(denominator){
			return std::make_pair( ( b*v - c*u )/denominator, ( b*u - a*v )/denominator );
		}else{
			return std::pair<scalar_type, scalar_type>( 0, 0 );
		}
	}

	template<typename _FlowVectorComponentImage, typename _Image, typename _WeightFunction, typename _WindowRadius>
	void OpticalFlow( 
		_FlowVectorComponentImage &outx,
		_FlowVectorComponentImage &outy,
		const _Image &current,
		const _Image &next,
		const _WeightFunction &weightFunc,
		const _WindowRadius &windowRad
		)
	{
		size_t sX(SizeX(current));
		size_t sY(SizeY(current));
		assert(SizeX(current) == SizeX(outx));
		assert(SizeY(current) == SizeY(outx));
		assert(SizeX(current) == SizeX(outy));
		assert(SizeY(current) == SizeY(outy));
		assert(SizeX(current) == SizeX(next));
		assert(SizeY(current) == SizeY(next));
		for(size_t x(0); x<sX; ++x){
			for(size_t y(0); y<sY; ++y){
				typedef typename FunctionTraits<_Image>::ValueType scalar_type;
				const std::pair<scalar_type,scalar_type> r(OpticalFlowAtPoint(current,next,weightFunc,windowRad,x,y));
				outx(x,y) = r.first;
				outy(x,y) = r.second;
			}
		}
	}
};
