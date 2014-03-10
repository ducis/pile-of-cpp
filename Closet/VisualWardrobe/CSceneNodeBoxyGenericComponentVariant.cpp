#include "CSceneNodeBoxyGenericComponentVariant.h"
#include "UnifiedCoord.h"

namespace CSceneNodeBoxyGenericComponentVariant_impl{}
using namespace CSceneNodeBoxyGenericComponentVariant_impl;


namespace CSceneNodeBoxyGenericComponentVariant_impl{
	const UnifiedCoord<vector3> GetRemapMin(const vector3 &size, unsigned bindingFlags){
		vector3 scale;
		vector3 offset;
		for( unsigned iAxis(0); iAxis<3; ++iAxis ){
			if((bindingFlags>>iAxis)&1){
				scale[iAxis] = 1;
				offset[iAxis] = -size[iAxis];
			}else{
				scale[iAxis] = offset[iAxis] = 0;
			}
		}
		return UnifiedCoord<vector3>(scale,offset);
	}
	const UnifiedCoord<vector3> GetRemapMax(const vector3 &size, unsigned bindingFlags){
		vector3 scale;
		vector3 offset;
		for( unsigned iAxis(0); iAxis<3; ++iAxis ){
			if((bindingFlags>>iAxis)&1){
				scale[iAxis] = 1;
				offset[iAxis] = 0;
			}else{
				scale[iAxis] = 0;
				offset[iAxis] = size[iAxis];
			}
		}
		return UnifiedCoord<vector3>(scale,offset);
	}
};

CSceneNodeBoxyGenericComponentVariant::CSceneNodeBoxyGenericComponentVariant(
	CSceneNodeBoxyGenericComponent &pComponent, 
	const VariantValueType &variant, 
	unsigned bindingFlags
	)
	:
m_pComponent(&pComponent), 
m_variant(variant), 
m_repositioner( &pComponent, GetRemapMin(variant.second,bindingFlags), GetRemapMax(variant.second,bindingFlags) )
{
	m_repositioner.Set(CSceneNodeBoxyRepositioner::Enclosing);
}

