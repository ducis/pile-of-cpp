#pragma once

#include "ISceneNodeBoxy.h"
#include "IDraggable.h"
#include <vector>
#include <memory>
#include "CDraggableAABBSurface.h"
#include "CObjectConstRef.h"
#include "IConnectible.h"

class CSceneNodeBoxyDraggableContainer:public ISceneNodeBoxy, public IPickable, public IDraggable, public IConnectible{
public:
	//�ṩ��ק�ƶ�,�Զ���ע����
	CSceneNodeBoxyDraggableContainer(float rotZInDeg,const vector3 &translation,bool bCreateConstraint);
	~CSceneNodeBoxyDraggableContainer();
	void RipObjectSet(class CSceneNodeObjectSet *pObjectSet);//��pObjectSet��ȡ���ӽڵ�
	virtual void Render() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj);
	virtual float Pick(const vector2 &posHS, const matrix44 &viewProj) const;
	virtual void VisualizePick() const;
	virtual void OnInsertion();
	void Rotate90Deg();//��z��
	virtual void UpdateTransform(const matrix44 &parentTransform);
	void SetSurfaceDraggable(int mask);//6������(+x,-x,+y,-y,+z,-z)�ϵ���ק�ı��С
	void EnableAutoAnnotation();
	typedef CDraggableAABBSurface< CObjectConstRef<class matrix44> > DraggableSurfaceType;
	void Insert(ISceneNodeBoxy *p);
	void Clear();
	virtual bool ConnectTo(const IConnectible &theOther);//�ܹ���Connectible�ĵ��ô�����Connectible�ӿڵ��ֽڵ���
	virtual bool DisconnectFrom(const IConnectible &theOther);
private:
	void SetAABB_impl(const AABB &aabb,int from);
	void SetAABBThis(const AABB &aabb);
	class CAxialAnnotator;
	std::auto_ptr< CAxialAnnotator > m_axialAnnotator;
	std::auto_ptr< DraggableSurfaceType > m_draggableSurfaces[6];
	//ISceneNodeBoxy *m_pContained;
	float m_rotZInDeg;
	vector3 m_translation;
	std::vector<ISceneNodeBoxy*> m_pContained;
	const matrix44 &GetTransform() const;
	matrix44 m_parentTransform;
	mutable matrix44 m_worldTransform;
	std::auto_ptr<class CConstraintOutOfAABB> m_pConstraint;
	CSceneNodeBoxyDraggableContainer *m_pConnectedOnRight;
	CSceneNodeBoxyDraggableContainer *m_pConnectedOnLeft;

	void ConnectChild(IConnectible *p);
	void DisconnectChild(IConnectible *p);
	static void ConnectChild(IConnectible *p, const CSceneNodeBoxyDraggableContainer &target);
	static void DisconnectChild(IConnectible *p, const CSceneNodeBoxyDraggableContainer &target);
};
