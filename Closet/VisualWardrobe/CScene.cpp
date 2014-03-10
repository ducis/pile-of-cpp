#include "CScene.h"
#include "ISceneNode.h"
#include <cassert>

using namespace std;
void CScene::Render(){
	for(list<ISceneNode *>::iterator itr = m_listSceneNodes.begin() ; itr != m_listSceneNodes.end() ; ++itr){
		(*itr)->Render();
	}
}

void CScene::Insert(ISceneNode *pSceneNode){
	assert(pSceneNode);
	if(!pSceneNode)
		return;
	m_listSceneNodes.push_back(pSceneNode);
	pSceneNode->UpdateTransform(IdentityMatrix44());
	pSceneNode->OnInsertion();
}

void CScene::Remove(class ISceneNode *pSceneNode){
	m_listSceneNodes.remove(pSceneNode);
}

CScene::CScene(){
}

const AABB CScene::GetAABB() const{
	AABB r(MinimumAABB());
	for(std::list<class ISceneNode *>::const_iterator i(m_listSceneNodes.begin()); i!=m_listSceneNodes.end(); ++i){
		(*i)->EnlargeAABB(r);
	}
	return r;
}
