#pragma once

#include "ISceneNodeBoxy.h"
#include "CSceneNodeAABB.h"
#include "CBoard.h"
#include "CTexCoordGen.h"
#include "IPickable.h"
#include "boost/property_tree/ptree_fwd.hpp"
#include "IAnnotated.h"
#include "IMaterialUser.h"


class CSceneNodeBoard: public ISceneNodeBoxy, public IPickable, public IAnnotated, public IMaterialUser{
public:
	//在场景中显示板材的类
	CSceneNodeBoard(class IMaterial *pMaterial,const AABB &aabb,unsigned up, class CSceneNodeBinaryWardrobeTreeNode * pOwner, 
		ISceneNodeBoxy *pVisual);
	~CSceneNodeBoard();
	virtual void Render() const;
	virtual void VisualizePick() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	const CBoard &GetBoard() const;
	void PunchHoles();
	virtual void SetMaterial( class IMaterial *pMaterial ){ SetBoardMaterial(pMaterial); }
	void SetBoardMaterial (class IMaterial *pMaterial);
	void SetBoardName(const std::wstring &s) { m_board.SetName(s); }
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	int GetDirection(int localDirection);
	int GetLocalDirection(int direction);
	void CheckDrillingRules();
	void ReadProps(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p);//从generator和manufactureProperties读取板子的厚度等信息
	static ISceneNodeBoxy *CreateDefaultVisual(const AABB &aabb);
	void GiveVisual(ISceneNodeBoxy *pVisual);//设置显示方式
	virtual void ShowAnnotation() const;
	void Enable();
	void Disable();
	void SetBoardRemovable(bool removable);//设置是否可删
	bool GetBoardRemovable()const{ return m_bRemovable; }
	void SetWardrobe(class CSceneNodeWardrobe *pWardrobe){ m_pWardrobe = pWardrobe; }
private:
	class CSceneNodeWardrobe *m_pWardrobe;
	bool m_bRemovable;
	CSceneNodeBoard(const CSceneNodeBoard &);
	const AABB GetModifiedAABB() const;
	AABB m_aabb;
	std::pair<vector3,vector3> m_boardSpatialMod;
	class IDrillingRule *m_pDrillingRuleOverride[2][2];
	bool m_overrideDrillingRule[2][2];
	static unsigned TransformDirection(unsigned direction, unsigned up);
	static unsigned InverseTransformDirection(const unsigned direction, const unsigned up);
	static unsigned TransformAxis(unsigned iAxis, unsigned up);
	class CSceneNodeBinaryWardrobeTreeNode *m_pOwner;
	matrix44 m_worldTransform;
	static const vector4 UCoeff(int iAxis);
	static const vector4 VCoeff(int iAxis);
	int m_up;
	//CSceneNodeAABB m_sceneNodeAABB;
	std::auto_ptr<ISceneNodeBoxy> m_pVisual;
	CBoard m_board;
	CTexCoordGen m_texCoordGen;
	bool CanBeRemovable();
};

namespace NSceneNodeBoard{
	inline CSceneNodeBoard *New(class IMaterial *pMaterial,const AABB &aabb,unsigned up,class CSceneNodeBinaryWardrobeTreeNode *pOwner){
		return new CSceneNodeBoard(pMaterial,aabb,up,pOwner,CSceneNodeBoard::CreateDefaultVisual(aabb));
	}
}
