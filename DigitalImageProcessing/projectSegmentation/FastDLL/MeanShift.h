#pragma once

template<typename T> inline void SetToZero(T &x){
	x = 0;
}

#include "FixedVector.h"
#include "Matrix.h"
#include <cmath>
#include <utility>
#include <functional>
#include <fstream>
#include <cassert>
#include <string>
#include <map>
#include <queue>
#include <functional>

#define _AVOID_CONV

namespace MeanShift{
	template< typename S >
	class EpanechnikovKernel: public std::unary_function<S,S>{
	public:
		inline const S operator()(const S &x)const{
			return x>=S(1.0) ? S(0.0) : S(0.75)*(S(1.0)-x*x);
		}
	};

	template< typename S >
	class UniformKernel: public std::unary_function<S,S>{
	public:
		inline const S operator()(const S &x)const{
			return x>=S(1.0) ? S(0.0) : S(1.0);
		}
	};


	template< typename S, typename V, typename _KernelFunc >
	inline const std::pair<FixedVector<S,2>,V> JointDomainMean(
		const Matrix<V> &img,
		std::pair<FixedVector<S,2>,V> cur,
		size_t searchRadius,
		const S &oneOverSpatialBandwidthSquared,
		const S &oneOverColorBandwidthSquared,
		const _KernelFunc &kernelFunc
		)
	{
		size_t row( size_t(cur.first[0]) );
		size_t col( size_t(cur.first[1]) );
		S sumWeights(0);
		typedef FixedVector<S,2> spatial_type;
		spatial_type sumWeightedSpatials;
		SetToZero(sumWeightedSpatials);
		V sumWeightedColors;
		SetToZero(sumWeightedColors);
		size_t rowFirst( std::max(signed(row)-signed(searchRadius),0) );
		size_t rowLast( std::min(row+searchRadius+1,img.NumRows()) );
		size_t colFirst( std::max(signed(col)-signed(searchRadius),0) );
		size_t colLast( std::min(col+searchRadius+1,img.NumColumns()) );

#ifdef _DBGOUT
		std::wofstream(L"DPS.txt",std::ios_base::app)
			<<std::wstring(L"==========\n")
			<<std::wstring(L"cur.first ")<<cur.first<<std::endl
			<<std::wstring(L"cur.second ")<<cur.second<<std::endl
			<<std::wstring(L"----------\n")<<std::endl
			;
#endif

#ifdef _AVOID_CONV
		S sR(rowFirst);
		S sC(colFirst);
		spatial_type newSpatial;
		newSpatial[0] = rowFirst;
		for( size_t r(rowFirst); r<rowLast; ++r,newSpatial[0]+=S(1.0) ){
			newSpatial[1] = colFirst;
			for( size_t c(colFirst); c<colLast; ++c,newSpatial[1]+=S(1.0) ){
#else
		for( size_t r(rowFirst); r<rowLast; ++r ){
			for( size_t c(colFirst); c<colLast; ++c ){
				const spatial_type newSpatial = { r,c };
#endif
				const V newColor( img.Get(r,c) );
				const S ds( ((newSpatial-cur.first).NormSq()) * oneOverSpatialBandwidthSquared );
				const S dc( ((newColor-cur.second).NormSq()) * oneOverColorBandwidthSquared );
				const S w( kernelFunc(ds)*kernelFunc(dc) );
				sumWeightedSpatials += w*newSpatial;
				sumWeightedColors += w*newColor;
				sumWeights += w;
				
#ifdef _DBGOUT
				std::wofstream(L"DPS.txt",std::ios_base::app)
					<<std::wstring(L"r ")<<r<<std::endl
					<<std::wstring(L"c ")<<c<<std::endl
					<<std::wstring(L"newSpatial ")<<newSpatial<<std::endl
					<<std::wstring(L"newColor ")<<newColor<<std::endl
					<<std::wstring(L"(newSpatial-cur.first) ")<<(newSpatial-cur.first)<<std::endl
					<<std::wstring(L"(newColor-cur.second) ")<<(newColor-cur.second)<<std::endl
					<<std::wstring(L"((newSpatial-cur.first).NormSq()) ")<<((newSpatial-cur.first).NormSq())<<std::endl
					<<std::wstring(L"((newColor-cur.second).NormSq()) ")<<((newColor-cur.second).NormSq())<<std::endl
					<<std::wstring(L"ds ")<<ds<<std::endl
					<<std::wstring(L"dc ")<<dc<<std::endl
					<<std::wstring(L"w ")<<w<<std::endl
					;
#endif

			}
		}

#ifdef _DBGOUT
		{//DBG
			std::wofstream(L"DPS.txt",std::ios_base::app)
				<<std::wstring(L"sumWeights ")<<sumWeights<<std::endl
				<<std::wstring(L"sumWeightedSpatials ")<<sumWeightedSpatials<<std::endl
				<<std::wstring(L"sumWeightedColors ")<<sumWeightedColors<<std::endl
				<<std::wstring(L"sumWeightedSpatials/sumWeights ")<<sumWeightedSpatials/sumWeights<<std::endl
				<<std::wstring(L"sumWeightedColors/sumWeights ")<<sumWeightedColors/sumWeights<<std::endl
				<<std::endl;


			//sumWeightedColors.Set(sumWeights);
			//return std::make_pair(sumWeightedSpatials/sumWeights,sumWeightedColors);
		}
#endif

		return std::make_pair(sumWeightedSpatials/sumWeights,sumWeightedColors/sumWeights);
	}

	template< typename S, typename V >
	inline bool CloseEnough(
		const std::pair< FixedVector<S,2> , V> &a,
		const std::pair< FixedVector<S,2> , V> &b,
		const S &oneOverSpatialBandwidthSquared,
		const S &oneOverColorBandwidthSquared,
		const S &convergenceDistSqr
		)
	{
		return 
			(a.first-b.first).NormSq()*oneOverSpatialBandwidthSquared
			+(a.second-b.second).NormSq()*oneOverColorBandwidthSquared
			<convergenceDistSqr;
	}

	template< typename S, typename V >
	inline const std::pair<FixedVector<S,2>,V> FilterPixel(
		const Matrix<V> &img,
		size_t row,
		size_t col,
		size_t searchRadius,
		const S &oneOverSpatialBandwidthSquared,
		const S &oneOverColorBandwidthSquared,
		unsigned maxIteration,
		const S &convergenceDistSqr)
	{
		//searchRadius = 1;///////
		typedef FixedVector<S,2> spatial_type;
		const spatial_type spatial = {row,col};

		const V color( img.Get(row,col) );

		typedef std::pair<spatial_type,V> joint_type;
		joint_type cur( spatial, color );
		for(unsigned i(0); i<maxIteration; ++i){
			joint_type mean( JointDomainMean(img, cur, searchRadius, oneOverSpatialBandwidthSquared, oneOverColorBandwidthSquared, UniformKernel<S>()) );
			bool stop( CloseEnough(cur,mean,oneOverSpatialBandwidthSquared,oneOverColorBandwidthSquared,convergenceDistSqr) );
			cur = mean;
			if(stop) break;
		}
		return cur;
	}


	template< typename S, typename V >
	const Matrix<V> DiscontinuityPreservingSmooth( 
		const Matrix<V> &img, 
		const S &spatialBandwidth, 
		const S &colorBandwidth, 
		unsigned maxIteration,
		const S &convergenceDistSqr )
	{
		Matrix<V> smoothed(img.NumRows(),img.NumColumns());
		const size_t searchRadius(spatialBandwidth);
		const S oneOverSpatialBandwidthSquared(S(1.0)/pow(spatialBandwidth,2));
		const S oneOverColorBandwidthSquared(S(1.0)/pow(colorBandwidth,2));
#ifdef _DBGOUT
		//DBG-----
		std::wofstream(L"DPS.txt")
			<<L"searchRadius "<<searchRadius<<std::endl
			<<L"oneOverSpatialBandwidthSquared "<<oneOverSpatialBandwidthSquared<<std::endl
			<<L"oneOverColorBandwidthSquared "<<oneOverColorBandwidthSquared<<std::endl
			;
		//--------
#endif
		for(size_t c(0); c<img.NumColumns(); ++c){
			for(size_t r(0); r<img.NumRows(); ++r){
				smoothed.Set( r, c, FilterPixel(
					img,
					r,
					c,
					searchRadius,
					oneOverSpatialBandwidthSquared,
					oneOverColorBandwidthSquared,
					maxIteration,
					convergenceDistSqr).second);
				//smoothed.Set( r, c, img.Get(r,c)/2 );
			}
		}
		return smoothed;
	}



	namespace Delineate_impl{


		template< typename I, typename S, typename V, typename _SizeType >
		inline void Try(
			Matrix<I> &segIdMap,
			std::queue<std::pair<_SizeType,_SizeType>> &q,
			const std::pair< FixedVector<S,2>, V > &testAgainst,
			const _SizeType &row,
			const _SizeType &col,
			const Matrix< std::pair<FixedVector<S,2>,V> > &jointImg,
			const S &spatialBandwidthSquared, 
			const S &colorBandwidthSquared,	
			const I &toFillOver,
			const I &toFillWith
			)
		{
			if( 
				( toFillOver == segIdMap.Get(row,col) )
				&& ( (testAgainst.first - jointImg.Get(row,col).first).NormSq() < spatialBandwidthSquared )
				&& ( (testAgainst.second - jointImg.Get(row,col).second).NormSq() < colorBandwidthSquared )
				)
			{
				segIdMap.Set(row,col,toFillWith);
				q.push(std::make_pair(row,col));
			}
		}


		template< typename I, typename S, typename V, typename _SizeType >
		inline void FloodFill(
			Matrix<I> &segIdMap,
			std::queue<std::pair<_SizeType,_SizeType>> &q,
			const Matrix< std::pair<FixedVector<S,2>,V> > &jointImg,
			const S &spatialBandwidthSquared, 
			const S &colorBandwidthSquared,	
			const I &toFillOver,
			const I &toFillWith,
			const _SizeType &startRow,
			const _SizeType &startCol
			)
		{
			typedef std::pair<_SizeType,_SizeType> coord_type;
			segIdMap.Set(startRow, startCol, toFillWith);
			q.push(coord_type(startRow,startCol));
			_SizeType nR(jointImg.NumRows());
			_SizeType nC(jointImg.NumColumns());
			while(!q.empty()){
				size_t r(q.front().first);
				size_t c(q.front().second);
				q.pop();
#define TRY( _ROW, _COL ) Try(segIdMap, q, jv, (_ROW), (_COL), jointImg, spatialBandwidthSquared, colorBandwidthSquared, toFillOver, toFillWith )
				const std::pair<FixedVector<S,2>,V> &jv(jointImg.Get(r,c));
				if(r>0){
					TRY(r-1,c);
					if(c>0){
						TRY(r-1,c-1);
					}
					if(c+1<nC){
						TRY(r-1,c+1);
					}
				}
				{
					if(c>0){
						TRY(r,c-1);
					}
					if(c+1<nC){
						TRY(r,c+1);
					}
				}
				if(r+1<nR){
					TRY(r+1,c);
					if(c>0){
						TRY(r+1,c-1);
					}
					if(c+1<nC){
						TRY(r+1,c+1);
					}
				}
#undef TRY
			}
		}
	}

	template< typename I, typename S, typename V>
	const Matrix<I> Delineate( 
		const Matrix< std::pair<FixedVector<S,2>,V> > &jointImg,
		const S &spatialBandwidthSquared, 
		const S &colorBandwidthSquared,	
		const I &initialId,
		const I &invalidId
		)
	{
		using namespace Delineate_impl;
		size_t nR(jointImg.NumRows());
		size_t nC(jointImg.NumColumns());
		Matrix<I> ret(nR,nC,invalidId);
		I curId(initialId);
		typedef std::pair<size_t,size_t> coord_type;
		typedef std::queue<coord_type> queue_type;
		queue_type q;
		for(size_t r(0); r<nR; ++r){
			for(size_t c(0); c<nC; ++c){
				if(invalidId == ret.Get(r,c)){
					FloodFill(ret,q,jointImg,spatialBandwidthSquared,colorBandwidthSquared,invalidId,curId++,r,c);
				}
			}
		}
		return ret;
	}


	template< typename C >
	Matrix<C> &RepaintSmallRegions_Inplace( Matrix<C> &img, const unsigned long minRegionSize, const C &repaintColor ){
		std::map<C,unsigned long> counts;
		for(Matrix<C>::ext_const_iterator_type itr(img.Begin()); itr!= img.End(); ++itr){
			++counts[*itr];
		}
		for(Matrix<C>::ext_iterator_type itr(img.Begin()); itr!= img.End(); ++itr){
			if(counts[*itr]<minRegionSize)	*itr = repaintColor;
		}
		return img;
	}

	template< typename C >
	const Matrix<C> RepaintSmallRegions( const Matrix<C> &img, const unsigned long &minRegionSize,const C &repaintColor ){
		Matrix<C> repainted(img);
		RepaintSmallRegions_Inplace( repainted, minRegionSize, repaintColor );
		return repainted;
	}


	//template< typename _Id, typename _ImgValue, typename _DistFunc >
	//Matrix<_Id> &PruneRegionsById_Inplace( Matrix<_Id> &segIdMap, const Matrix<_ImgValue> &img, const _DistFunc &distFunc, const _Id &prunedId ){
	//	typedef std::pair<size_t,size_t> coord_type;

	//	class DistGreater:public std::binary_function<const coord_type &, const coord_type &, bool>{
	//	public:
	//		bool operator()(const coord_type &l, const coord_type &r){
	//			return distFunc(img.Get(l.first,l.second))>distFunc(img.Get(r.first,r.second));
	//		}
	//	};
	//	typedef std::priority_queue<coord_type,std::vector<coord_type>,DistGreater> pq_type;
	//	pq_type pq;
	//	for(size_t r(0);r<segIdMap.NumRows();++r){
	//		for(size_t c(0);c<segIdMap.NumColumns();++c){
	//			if( segIdMap.Get(r,c)!=prunedId && HasNeighboringValue( segIdMap, r, c, prunedId ) ){
	//				pq.push(coord_type(r,c));
	//			}
	//		}
	//	}
	//	while(!pq_type.empty()){
	//		size_t r( pq.top().first );
	//		size_t c( pq.top().second );
	//		pq.pop();
	//	}
	//}


	//template< typename _Id, typename _ImgValue, typename _DistFunc >
	//const Matrix<_Id> PruneRegionsById( const Matrix<_Id> &segIdMap, const Matrix<_ImgValue> &img, const _DistFunc &distFunc, const _Id &prunedId ){
	//	Matrix<_Id> pruned(segIdMap);
	//	PruneRegionsById_Inplace( pruned, img, distFunc );
	//	return pruned;
	//}

	//template< typename _Id, typename _ImgValue, typename _DistFunc >
	//Matrix<_Id> &PruneRegionsById_Inplace( Matrix<_Id> &segIdMap, const Matrix<_ImgValue> &img, const _DistFunc &distFunc, const _Id &prunedId ){
	//	typedef std::pair<size_t,size_t> coord_type;


	template< typename _Id >
	Matrix<_Id> &PruneRegionsById_Inplace( Matrix<_Id> &segIdMap, const _Id &pruneId ){
		typedef std::pair<size_t,size_t> coord_type;
		typedef std::queue<coord_type> queue_type;
		queue_type q;
		size_t nR(segIdMap.NumRows());
		size_t nC(segIdMap.NumColumns());
		for(size_t r(0); r<segIdMap.NumRows(); ++r){
			for(size_t c(0); c<segIdMap.NumColumns(); ++c){
				if(segIdMap.Get(r,c) != pruneId){
					if(
						(r>0 && segIdMap.Get(r-1,c)!=pruneId)
						||(r+1<nR && segIdMap.Get(r+1,c)!=pruneId)
						||(c>0 && segIdMap.Get(r,c-1)!=pruneId)
						||(c+1<nC && segIdMap.Get(r,c+1)!=pruneId)
						)
					{
						q.push(coord_type(r,c));
					}
				}
			}
		}
		while(!q.empty()){
			size_t r(q.front().first);
			size_t c(q.front().second);
			q.pop();
			_Id id( segIdMap.Get(r,c) );
#define TRY(_ROW,_COL) if( segIdMap.Get( (_ROW) , (_COL) ) == pruneId ){    segIdMap.Set( (_ROW) , (_COL) , id );  q.push( coord_type( (_ROW), (_COL) ) );  }
			if(r>0){
				TRY(r-1,c);
			}
			if(r+1<nR){
				TRY(r+1,c);
			}
			if(c>0){
				TRY(r,c-1);
			}
			if(c+1<nC){
				TRY(r,c+1);
			}
#undef TRY
		}
		return segIdMap;
	}

	template< typename _Id >
	const Matrix<_Id> PruneRegionsById( const Matrix<_Id> &segIdMap, const _Id &pruneId ){
		Matrix<_Id> pruned(segIdMap);
		PruneRegionsById_Inplace( pruned , pruneId );
		return pruned;
	}

	template< typename S, typename V >
	const Matrix<unsigned long> Segment( 
		const Matrix<V> &img, 
		const S &spatialBandwidth, 
		const S &colorBandwidth, 
		unsigned long minRegionSize,
		unsigned maxIteration,
		const S &convergenceDistSqr)
	{
		const size_t searchRadius(spatialBandwidth);
		const S oneOverSpatialBandwidthSquared(S(1.0)/pow(spatialBandwidth,2));
		const S oneOverColorBandwidthSquared(S(1.0)/pow(colorBandwidth,2));
		Matrix< std::pair<FixedVector<S,2>,V> > filtered(img.NumRows(),img.NumColumns());
		for(size_t c(0); c<img.NumColumns(); ++c){
			for(size_t r(0); r<img.NumRows(); ++r){
				filtered.Set( r, c, FilterPixel(
					img,
					r,
					c,
					searchRadius,
					oneOverSpatialBandwidthSquared,
					oneOverColorBandwidthSquared,
					maxIteration,
					convergenceDistSqr));
			}
		}
		Matrix< unsigned long > r( Delineate(filtered,pow(spatialBandwidth,2),pow(colorBandwidth,2),0UL,0xFFFFFFFFUL) );
		//Matrix<unsigned long> r(img.NumRows(),img.NumColumns(),1500);

		return r;

		//return RepaintSmallRegions( 
		//	r,
		//	minRegionSize,
		//	0xFFFFFFFFUL
		//	);
	}






	template< typename S, size_t L1, size_t L2 >
	const S DistSqr(const std::pair< FixedVector<S,L1>,FixedVector<S,L2> > &l, const std::pair< FixedVector<S,L1>,FixedVector<S,L2> > &r){
		return DistSqr(l.first, r.first)+DistSqr(l.second, r.second);
	}

	template< typename S, typename V1, typename V2 >
	const S FarEnough(const std::pair<V1,V2> &l, const std::pair<V1,V2> &r, const S &thresholdV1Squared, const S &thresholdV2Squared){
		return ( DistSqr(l.first, r.first) > thresholdV1Squared )
				|| ( DistSqr(l.second, r.second) > thresholdV2Squared );
	}


	template< typename S, typename V >
	const Matrix<S> Edgeness(
		const Matrix< std::pair<FixedVector<S,2>,V> > &jointImg,
		const S &spatialBandwidthSquared,
		const S &colorBandwidthSquared
		)
	{
		size_t nR(jointImg.NumRows());
		size_t nC(jointImg.NumColumns());
		//0'-'  1'|'  2'\'  3'/'
		Matrix< FixedVector<S,4> > edgeness( nR, nC );
		for(size_t c(0); c<nC; ++c){
			for(size_t r(0); r<nR; ++r){
				const std::pair<FixedVector<S,2>,V> &cur(jointImg.Get(r,c));
				FixedVector<S,4> ev = {0,0,0,0};
#define DELTA(_ROW,_COL) (FarEnough(jointImg.Get((_ROW),(_COL)),cur,spatialBandwidthSquared,colorBandwidthSquared)?S(1):S(0))
				if(r>0){
					ev[1]+=DELTA(r-1,c);
					if(c>0){
						ev[2]+=DELTA(r-1,c-1);
					}
					if(c+1<nC){
						ev[3]+=DELTA(r-1,c+1);
					}
				}
				if(c>0){
					ev[0]+=DELTA(r,c-1);
				}
				if(c+1<nC){
					ev[0]+=DELTA(r,c+1);
				}
				if(r+1<nR){
					ev[1]+=DELTA(r+1,c);
					if(c>0){
						ev[3]+=DELTA(r+1,c-1);
					}
					if(c+1<nC){
						ev[2]+=DELTA(r+1,c+1);
					}
				}
				edgeness.Set(r,c,ev);
#undef DELTA
			}
		}
		Matrix< S > filteredEdgeness( nR, nC );
		for(size_t c(0); c<nC; ++c){
			for(size_t r(0); r<nR; ++r){
				FixedVector<S,4> filtered = S(2)*edgeness.Get(r,c);
				if(r>0){
					filtered[1]-=S(0.5)*edgeness.Get(r-1,c)[1];
					if(c>0){
						filtered[2]-=S(0.5)*edgeness.Get(r-1,c-1)[2];
					}
					if(c+1<nC){
						filtered[3]-=S(0.5)*edgeness.Get(r-1,c+1)[3];
					}
				}
				if(c>0){
					filtered[0]-=S(0.5)*edgeness.Get(r,c-1)[0];
				}
				if(c+1<nC){
					filtered[0]-=S(0.5)*edgeness.Get(r,c+1)[0];
				}
				if(r+1<nR){
					filtered[1]-=S(0.5)*edgeness.Get(r+1,c)[1];
					if(c>0){
						filtered[3]-=S(0.5)*edgeness.Get(r+1,c-1)[3];
					}
					if(c+1<nC){
						filtered[2]-=S(0.5)*edgeness.Get(r+1,c+1)[2];
					}
				}
				//filtered = edgeness.Get(r,c);
				std::sort(filtered.Begin(),filtered.End());
				//filteredEdgeness.Set(r,c,filtered[3]+filtered[2]-filtered[1]-filtered[0]);
				filteredEdgeness.Set(r,c,filtered[3]+filtered[2]);
				//filteredEdgeness.Set(r,c,*std::max_element(filtered.Begin(),filtered.End()));
			}
		}
		return filteredEdgeness;
	}


	template< typename S, typename V >
	const Matrix<S> EdgenessMap(
		const Matrix<V> &img, 
		const S &spatialBandwidth, 
		const S &colorBandwidth,
		unsigned maxIteration,
		const S &convergenceDistSqr)
	{
		const size_t searchRadius(spatialBandwidth);
		const S oneOverSpatialBandwidthSquared(S(1.0)/pow(spatialBandwidth,2));
		const S oneOverColorBandwidthSquared(S(1.0)/pow(colorBandwidth,2));
		Matrix< std::pair<FixedVector<S,2>,V> > filtered(img.NumRows(),img.NumColumns());
		for(size_t c(0); c<img.NumColumns(); ++c){
			for(size_t r(0); r<img.NumRows(); ++r){
				filtered.Set( r, c, FilterPixel(
					img,
					r,
					c,
					searchRadius,
					oneOverSpatialBandwidthSquared,
					oneOverColorBandwidthSquared,
					maxIteration,
					convergenceDistSqr));
			}
		}

		//Matrix< unsigned char > r(img.NumColumns(),img.NumRows(),'0');
		//PaintEdge_Inplace(r,filtered,pow(spatialBandwidth,2),pow(colorBandwidth,2),edgeThreshold,unsigned char(0xFF));
		return Edgeness(filtered,pow(spatialBandwidth,2),pow(colorBandwidth,2));
	}
}
