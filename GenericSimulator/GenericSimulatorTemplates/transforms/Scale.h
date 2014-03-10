#pragma once

namespace GenericSim{
	namespace TransformFunc{
		template<class _Vector, class _Coeff>
		_Vector Scale(const _Vector &vec, const _Coeff &coeff){
			return vec * coeff;
		}
		
		namespace Inplace{

			template<class _Vector, class _Coeff>
			_Vector &Scale(_Vector &vec, const _Coeff &coeff){
				return vec *=coeff;
			}

		}

	}
}
