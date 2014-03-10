#pragma once

#include <boost/property_tree/ptree_fwd.hpp> 
#include "ISceneNodeBoxy.h"
#include <memory>
#include "3d.h"
#include "IConnectible.h"


class CSceneNodeWardrobeSelector:public ISceneNodeBoxy, public IConnectible{
	//��������ڿ��ƹ�����໥����(����,����,������,����)����Ĺ������ı仯����
public:
	virtual bool ConnectTo(const IConnectible &theOther);
	virtual bool DisconnectFrom(const IConnectible &theOther);
	void Clear();
	CSceneNodeWardrobeSelector(
		const boost::property_tree::wptree &genStandalone,
		const boost::property_tree::wptree &genConnectedOnLeft,
		const boost::property_tree::wptree &genConnectedOnRight,
		const boost::property_tree::wptree &genConnectedOnBothSides,
		const boost::property_tree::wptree &manufactureParams);//��������������Ե�generator,���õ�manufactureProperties��һ����
	virtual void Render() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	virtual void EnlargeAABB(AABB &aabb) const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	virtual void OnInsertion();
	const class CSceneNodeBinaryWardrobeTreeNode *GetInNode(){ return m_pIn.get(); }//���ع������Ĳ���������Ĳ��ֵĸ��ڵ�
	const class CSceneNodeBinaryWardrobeTreeNode *GetOutNode(){ return m_pOut; }//һ���������Ĳ���������Ĳ���ֻ��һ���ڵ�����ٽ��������ӽڵ�
	void SetWardrobe(class CSceneNodeWardrobe *pWardrobe){ m_pWardrobe = pWardrobe; }//�����������Wardrobe��
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
