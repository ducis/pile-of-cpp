#include "CSceneNodeWardrobeSelector.h"
#include "singleton.h"
#include "CProgram.h"
#include "CSceneNodeBinaryWardrobeTreeNode.h"



const boost::property_tree::wptree &CSceneNodeWardrobeSelector::SelectGenerator(/*bool connectedOnLeft,bool connectedOnRight*/) const{
	if(m_bConnectedOnLeft){
		if(m_bConnectedOnRight){
			return m_connectedOnBothSides;
		}else{
			return m_connectedOnLeft;
		}
	}else{
		if(m_bConnectedOnRight){
			return m_connectedOnRight;
		}else{
			return m_standalone;
		}
	}
}


void CSceneNodeWardrobeSelector::Select(/*bool connectedOnLeft, bool connectedOnRight, */const boost::property_tree::wptree *pParams){
	if(!pParams) return;
	m_pManufactureParams = pParams;
	std::auto_ptr<CSceneNodeBinaryWardrobeTreeNode> pPrevIn(m_pIn);
	CSceneNodeBinaryWardrobeTreeNode *pPrevOut(m_pOut);
	std::pair<CSceneNodeBinaryWardrobeTreeNode *,CSceneNodeBinaryWardrobeTreeNode *> r(
		CSceneNodeBinaryWardrobeTreeNode::BuildBWT(SelectGenerator(/*connectedOnLeft,connectedOnRight*/),*m_pManufactureParams)
		);
	assert(r.first);
	assert(r.second);
	m_pIn.reset(r.first);
	m_pOut = r.second;
	if(pPrevIn.get()){
		SetAABB( pPrevIn->GetAABB() ); 
	}else{
		m_aabb = m_pIn->GetAABB();
	}
	m_pIn->UpdateTransform(m_worldTransform);
	{
		m_pIn->ForEachSeparatorOfType<CSceneNodeBoard>(std::bind2nd(std::mem_fun(&CSceneNodeBoard::SetWardrobe),m_pWardrobe));
	}
	if(pPrevOut)	m_pOut->ClaimContentsFrom(*pPrevOut);
}




CSceneNodeWardrobeSelector::CSceneNodeWardrobeSelector(
		const boost::property_tree::wptree &genStandalone,
		const boost::property_tree::wptree &genConnectedOnLeft,
		const boost::property_tree::wptree &genConnectedOnRight,
		const boost::property_tree::wptree &genConnectedOnBothSides,
		const boost::property_tree::wptree &manufactureParams):
m_pManufactureParams(&manufactureParams),
m_standalone(genStandalone),
m_connectedOnLeft(genConnectedOnLeft),
m_connectedOnRight(genConnectedOnRight),
m_connectedOnBothSides(genConnectedOnBothSides),
m_bConnectedOnLeft(false),m_bConnectedOnRight(false),
m_pOut(NULL),
m_pWardrobe(NULL)
{
	m_worldTransform.identity();
}



void CSceneNodeWardrobeSelector::Render() const{
	if(m_pIn.get()) m_pIn->Render();
}

void CSceneNodeWardrobeSelector::EnlargeAABB(AABB &aabb) const{
	if(m_pIn.get()) m_pIn->EnlargeAABB(aabb);
}

void CSceneNodeWardrobeSelector::SetAABB(const AABB &aabb){
	if(m_pIn.get()){
		m_pIn->SetAABB(aabb);
		m_aabb = m_pIn->GetAABB();
	}else{
		m_aabb = aabb;
	}
}
const AABB CSceneNodeWardrobeSelector::GetAABB() const{
	return m_aabb;
}
void CSceneNodeWardrobeSelector::UpdateTransform(const matrix44 &parentTransform){
	m_worldTransform = parentTransform;
	if(m_pIn.get()){
		m_pIn->UpdateTransform(parentTransform);
	}
}




bool CSceneNodeWardrobeSelector::ConnectTo(const IConnectible &theOther){
	const CSceneNodeWardrobeSelector *pOther( dynamic_cast<const CSceneNodeWardrobeSelector *>(&theOther) );
	if(!pOther) return false;
	for(int iAxis(0); iAxis<3; ++iAxis){
		if(!AreNormalizedAndInTheSameDirection(m_worldTransform.col[iAxis],pOther->m_worldTransform.col[iAxis])){
			return false;
		}
	}
	//matrix44 invTheOthersTransform( pOther->m_worldTransform );
	//invTheOthersTransform.invert();
	matrix44 invTransform(m_worldTransform);
	invTransform.invert();
	vector3 centerOfTheOthersAabb(invTransform*pOther->m_worldTransform*vector4(GetCenter(pOther->GetAABB()),1.0f));
	if(centerOfTheOthersAabb.y>GetCenter(GetAABB(),1)){
		if(!m_bConnectedOnRight){
			m_bConnectedOnRight = true;
			Select(m_pManufactureParams);
			return true;
		}
	}else{
		if(!m_bConnectedOnLeft){
			m_bConnectedOnLeft = true;
			Select(m_pManufactureParams);
			return true;
		}
	}
	return false;
}



bool CSceneNodeWardrobeSelector::DisconnectFrom(const IConnectible &theOther){
	const CSceneNodeWardrobeSelector *pOther( dynamic_cast<const CSceneNodeWardrobeSelector *>(&theOther) );
	if(!pOther) return false;
	for(int iAxis(0); iAxis<3; ++iAxis){
		if(!AreNormalizedAndInTheSameDirection(m_worldTransform.col[iAxis],pOther->m_worldTransform.col[iAxis])){
			return false;
		}
	}
	//matrix44 invTheOthersTransform( pOther->m_worldTransform );
	//invTheOthersTransform.invert();
	//vector3 centerOfTheOthersAabb(m_worldTransform*invTheOthersTransform*vector4(GetCenter(pOther->GetAABB()),1.0f));

	matrix44 invTransform(m_worldTransform);
	invTransform.invert();
	vector3 centerOfTheOthersAabb(invTransform*pOther->m_worldTransform*vector4(GetCenter(pOther->GetAABB()),1.0f));
	if(centerOfTheOthersAabb.y>GetCenter(GetAABB(),1)){
		if(m_bConnectedOnRight){
			m_bConnectedOnRight = false;
			Select(m_pManufactureParams);
			return true;
		}
	}else{
		if(m_bConnectedOnLeft){
			m_bConnectedOnLeft = false;
			Select(m_pManufactureParams);
			return true;
		}
	}
	return false;
}



void CSceneNodeWardrobeSelector::OnInsertion() { 
	Select(m_pManufactureParams); 
	if(m_pIn.get()) {
		m_pIn->OnInsertion();
		m_aabb = m_pIn->GetAABB();
	}
}