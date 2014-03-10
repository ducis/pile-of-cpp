#pragma once

#include "IPickable.h"
#include "IDraggable.h"
#include "IBoxy.h"
#include "CCoordSysAxis.h"
#include <functional>
#include <set>

template <class Transform>
class CDraggableAABBSurface:public IPickable, public IDraggable{//���������ڱ��϶�ʱ�ı�󶨵�IBoxy�ĳߴ����,�����϶���������Ĵ�С�����õ����
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
	//transform�ǰ󶨵�IBoxy�Ŀռ�任(���û�оʹ�����λ�����ȥ),pSelfConstraint�ǰ󶨵�IBoxy������е�Լ��(�϶�ʱ����ʱ���ô�Լ��)
	static Type IntToType( int i ){ return static_cast<Type>(i); }
	virtual void VisualizePick() const;
	void AddConstraintToDisable(class IConstraint *pConstraint) { m_pConstraintsToDisable.insert(pConstraint); }//��ӵ��϶�ʱ���õ�Լ��
	void RemoveConstraintToDisable(class IConstraint *pConstraint) { m_pConstraintsToDisable.erase(pConstraint); }//ʹԼ�����ٱ�����
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
