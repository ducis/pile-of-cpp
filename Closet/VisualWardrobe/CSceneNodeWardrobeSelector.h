#pragma once

#include <boost/property_tree/ptree_fwd.hpp> 
#include "ISceneNodeBoxy.h"
#include <memory>
#include "3d.h"
#include "IConnectible.h"


class CSceneNodeWardrobeSelector:public ISceneNodeBoxy, public IConnectible{
	//这个是用于控制柜体间相互连接(左连,右连,两侧连,不连)引起的柜体树的变化的类
public:
	virtual bool ConnectTo(const IConnectible &theOther);
	virtual bool DisconnectFrom(const IConnectible &theOther);
	void Clear();
	CSceneNodeWardrobeSelector(
		const boost::property_tree::wptree &genStandalone,
		const boost::property_tree::wptree &genConnectedOnLeft,
		const boost::property_tree::wptree &genConnectedOnRight,
		const boost::property_tree::wptree &genConnectedOnBothSides,
		const boost::property_tree::wptree &manufactureParams);//储存四种情况各自的generator,但用的manufactureProperties是一样的
	virtual void Render() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	virtual void EnlargeAABB(AABB &aabb) const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	virtual void OnInsertion();
	const class CSceneNodeBinaryWardrobeTreeNode *GetInNode(){ return m_pIn.get(); }//返回柜体树的不包括隔板的部分的根节点
	const class CSceneNodeBinaryWardrobeTreeNode *GetOutNode(){ return m_pOut; }//一个柜体树的不包括隔板的部分只有一个节点可以再接其他的子节点
	void SetWardrobe(class CSceneNodeWardrobe *pWardrobe){ m_pWardrobe = pWardrobe; }//设置相关联的Wardrobe类
private:
	class CSceneNodeWardrobe *m_pWardrobe;
	bool m_bConnectedOnLeft,m_bConnectedOnRight;
	AABB m_aabb;
	matrix44 m_worldTransform;
	std::auto_ptr<class CSceneNodeBinaryWardrobeTreeNode> m_pIn;
	class CSceneNodeBinaryWardrobeTreeNode *m_pOut;
	const boost::property_tree::wptree *m_pManufactureParams;
	const boost::property_tree::wptree &m_standalone;
	const boost::property_tree::wptree &m_connectedOnLeft;
	const boost::property_tree::wptree &m_connectedOnRight;
	const boost::property_tree::wptree &m_connectedOnBothSides;


	
	const boost::property_tree::wptree &SelectGenerator(/*bool connectedOnLeft,bool connectedOnRight*/) const;
	void Select(/*bool connectedOnLeft, bool connectedOnRight, */const boost::property_tree::wptree *pParams);
	//void Select(bool connectedOnLeft, bool connectedOnRight){
	//	Select(connectedOnLeft,connectedOnRight,m_pManufactureParams);
	//}
};
