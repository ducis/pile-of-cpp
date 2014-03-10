#pragma once
#include "ISceneNode.h"
#include "ptree.h"
#include "IControlPlane.h"
#include "mtxlib.h"
#include "CConstraintInsideOfAABB.h"
#include "Array.h"
#include "CAxialAABBAnnotation.h"
#include "CCutter.h"
#include "CTexCoordGen.h"

class CSceneNodeRoom: public ISceneNode{
public:
	//场景中的房间的类，包括天花板、墙壁、地板
	virtual void Render() const;
	virtual void OnInsertion();
	CSceneNodeRoom(const boost::property_tree::wptree &tr);//也是从xml生成的
	~CSceneNodeRoom();
	void SetFloorMaterial(class IMaterial *p){	m_pMtlFloor = p; }
	void SetWallMaterial(class IMaterial *p){  m_pMtlWalls = p; }
	void SetCeilingMaterial(class IMaterial *p){  m_pMtlCeiling = p; }
	virtual void EnlargeAABB(AABB &aabb) const;
private:
	CTexCoordGen m_texCoordGenXY,m_texCoordGenYZ,m_texCoordGenZX;
	class IMaterial *m_pMtlFloor,*m_pMtlWalls,*m_pMtlCeiling;
	UninitializedArray< CCutter, 4 > m_cutters;
	int m_sizeInMillimeters[3];
	int m_sizeCornerInMillimeters[4][2];
	class CControlPlane: public IControlPlane{
	public:
		CControlPlane(const CSceneNodeRoom &room,vector3 a,vector3 b);
		virtual void Render() const;
		virtual float Pick(const class vector2 &posHS,const class matrix44 &viewProj) const;
		virtual int Move(int delta) {return 0;}
		virtual int MoveTo(int p) {return 0;}
		virtual std::pair<float,float> CastRange( const class vector3 &dir ) const;
	private:
		const CSceneNodeRoom &m_room;
		vector3 m_vs[4];
	};
	CControlPlane *m_pCPs[2][6];
	CConstraintInsideOfAABB m_constraint;
	UninitializedArray< CAxialAABBAnnotation, 4*3 > m_axialAABBAnnotations;
};
