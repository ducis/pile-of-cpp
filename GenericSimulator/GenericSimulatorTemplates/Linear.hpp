#pragma once

namespace GenericSim{
	namespace TransformFunc{
		template<class _ArgColumnVector, class _ResultColumnVector, class _Matrix>
		_ResultColumnVector Linear(const _ArgColumnVector &arg, const _Matrix &mat){
			return mat * arg;
		}
		
		template<class _ColumnVector, class _Matrix>
		_ColumnVector LinearSqMat(const _ColumnVector &arg, const _Matrix &sqmat){
			return sqmat * arg;
		}
	}
}
