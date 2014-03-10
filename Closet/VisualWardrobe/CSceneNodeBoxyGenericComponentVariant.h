#pragma once

#include "CSceneNodeBoxyGenericComponent.h"
#include "CSceneNodeBoxyRepositioner.h"
#include <boost/property_tree/ptree_fwd.hpp>

class CSceneNodeBoxyGenericComponentVariant: public ISceneNodeBoxy{
public:
	//领带盒等一般部件的不同型号(指定其大小,还可以带有附加的信息),实质是用CSceneNodeBoxyRepositioner包装的CSceneNodeBoxyGenericComponent
	typedef std::wstring		VariantIdType;
	typedef vector3			VariantPropsType;
	typedef std::pair<VariantIdType, VariantPropsType> VariantValueType;

	virtual void EnlargeAABB(AABB &aabb) const{ m_repositioner.EnlargeAABB(aabb); }
	virtual void Render() const{ m_repositioner.Render(); }
	virtual void OnInsertion(){ m_repositioner.OnInsertion(); }
	virtual void UpdateTransform(const matrix44 &parentTransform){ m_repositioner.UpdateTransform(parentTransform); }
	virtual void SetAABB(const AABB &aabb){ m_repositioner.SetAABB(aabb); }
	virtual const AABB GetAABB() const{ return m_repositioner.GetAABB(); }
	CSceneNodeBoxyGenericComponentVariant(CSceneNodeBoxyGenericComponent &pComponent, const VariantValueType &variant, unsigned bindingFlags);//bindingFlags是传给CSceneNodeBoxyRepositioner的
private:
	CSceneNodeBoxyRepositioner m_repositioner;
	VariantValueType m_variant;
	CSceneNodeBoxyGenericComponent *m_pComponent;

};
