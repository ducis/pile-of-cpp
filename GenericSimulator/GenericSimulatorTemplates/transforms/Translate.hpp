#pragma once

namespace GenericSim{
	namespace TransformFunc{
		template<class _PositionVector, class _DisplacementVector>
		_PositionVector Translate(const _PositionVector &pos, const _DisplacementVector &disp){
			return pos+disp;
		}

		namespace Inplace{

			template<class _PositionVector, class _DisplacementVector>
			_PositionVector &Translate(_PositionVector &pos, const _DisplacementVector &disp){
				return pos+=disp;
			}

		}
	}
}