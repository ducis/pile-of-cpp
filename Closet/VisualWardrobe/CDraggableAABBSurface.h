#pragma once

#include "IPickable.h"
#include "IDraggable.h"
#include "IBoxy.h"
#include "CCoordSysAxis.h"
#include <functional>
#include <set>

template <class Transform>
class CDraggableAABBSurface:public IPickable, public IDraggable{//这是用于在被拖动时改变绑定的IBoxy的尺寸的类,比如拖动调整柜体的大小就是用的这个
public:
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const;
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj);
	enum Type{
		X_L,
		X_R,
		Y_L,
		Y_R,
		Z_L,
		Z_R
	};
	CDraggableAABBSurface( IBoxy *pBoxy, Type type, const Transform &transform, class IConstraint *pSelfConstraint);
	//transform是绑定的IBoxy的空间变换(如果没有就传个单位矩阵进去),pSelfConstraint是绑定的IBoxy自身带有的约束(拖动时会暂时禁用此约束)
	static Type IntToType( int i ){ return static_cast<Type>(i); }
	virtual void VisualizePick() const;
	void AddConstraintToDisable(class IConstraint *pConstraint) { m_pConstraintsToDisable.insert(pConstraint); }//添加的拖动时禁用的约束
	void RemoveConstraintToDisable(class IConstraint *pConstraint) { m_pConstraintsToDisable.erase(pConstraint); }//使约束不再被禁用
private:
	std::set<class IConstraint *> m_pConstraintsToDisable;
	const AABB GetPickingAABB() const;
	class IConstraint *m_pSelfConstraint;
	const Transform m_transform;
	IBoxy * const m_pBoxy;
	const Type m_type;
	int GetAxisId() const{ return m_type/2; }
	const CCoordSysAxis GetAxis() const{ return CCoordSysAxis(GetAxisId()); }
};
