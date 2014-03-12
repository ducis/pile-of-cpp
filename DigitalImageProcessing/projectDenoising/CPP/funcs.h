#pragma once

#include "Matrix.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>

namespace NonLocalDenoise_impl{
	template< typename S >
	inline const S EvaluateWeightedEucDist(
		const Matrix<S> &withNoise, 
		const Matrix<S> &gaussianWeights,
		int r1,
		int c1,
		int r2,
		int c2,
		int rSim
		)
	{
		S wed(0);
		for(int dc(-rSim); dc<=rSim; ++dc){
			for(int dr(-rSim); dr<=rSim; ++dr){
				wed+=pow(withNoise.Get(r1+dr,c1+dc)-withNoise.Get(r2+dr,c2+dc),2)*gaussianWeights.Get(dr+rSim,dc+rSim);
			}
		}
		return wed;
	}

	template< typename S >
	inline const S PixelOperation( 
		const Matrix<S> &withNoise, 
		const Matrix<S> &gaussianWeights, 
		int row, 
		int column, 
		int rFind, 
		int rSim,
		const S &oneOverHSquared,
		int firstR,
		int lastR,
		int firstC,
		int lastC
		)
	{
		int searchFirstR( max(row-rFind, firstR) );
		int searchLastR( min(row+rFind+1, lastR) );
		int searchFirstC( max(column-rFind, firstC) );
		int searchLastC( min(column+rFind+1, lastC) );
		S sumWeights(0);
		S sumWeightedColors(0);
		for(int c(searchFirstC); c<searchLastC; ++c){
			for(int r(searchFirstR); r<searchLastR; ++r){
				S w( exp(-oneOverHSquared*EvaluateWeightedEucDist(withNoise, gaussianWeights, row, column, r, c, rSim)) );
				sumWeights+=w;
				sumWeightedColors+=withNoise.Get(r,c)*w;
			}
		}
		return sumWeightedColors/sumWeights;
	};
};

template< typename S >
const Matrix<S> NonLocalDenoise( 
							 const Matrix<S> &withNoise, 
							 int firstR, 
							 int lastR, 
							 int firstC, 
							 int lastC, 
							 int rSearch,
							 int rSim,
							 const Matrix<S> &gaussianWeights,
							 const S &oneOverHSquared
							 )
{
	using namespace NonLocalDenoise_impl;
	Matrix<S> denoised( lastR-firstR , lastC-firstC );
	for(int c(firstC); c<lastC; ++c){
		for(int r(firstR); r<lastR; ++r){
			denoised.Set( r-firstR, c-firstC, 
				PixelOperation( withNoise, gaussianWeights, r, c, rSearch, rSim, oneOverHSquared, firstR, lastR, firstC, lastC )
				);
		}
	}
	return denoised;
}
