#include "CSceneNodeObjectSet.h"
#include "CSceneNodeFactoryRegistry.h"
#include "ISceneNodeFactory.h"
#include "singleton.h"
#include "CRegistryStack.h"
#include "CSceneNodePolygonModel.decl.h"
#include "CSceneNodeWallDef.h"
#include "CSceneNodeInteriorDef.h"
#include "CSceneNodeInterfaceDef.h"
void CSceneNodeObjectSet::Render() const{
	//for(std::vector<ISceneNode *>::const_iterator itr = m_children.begin(); itr!= m_children.end(); ++itr){
	//	(*itr)->Render();
	//}
	std::for_each(m_children.begin(),m_children.end(),std::mem_fun(&ISceneNode::Render)); 
}

CSceneNodeObjectSet::CSceneNodeObjectSet(const boost::property_tree::wptree &tr){
	GetSingleton<CRegistryStack<CSceneNodePolygonModel *>>()->Push();
	typedef std::stack<std::vector<CSceneNodeWallDef *>> WALLDEFSTACK;
	typedef std::stack<std::vector<CSceneNodeInteriorDef *>> INTERIORDEFSTACK;
	typedef std::stack<std::vector<CSceneNodeInterfaceDef *>> INTERFACEDEFSTACK;
	GetSingleton<WALLDEFSTACK>()->push(std::vector<CSceneNodeWallDef *>());
	GetSingleton<INTERIORDEFSTACK>()->push(std::vector<CSceneNodeInteriorDef *>());
	GetSingleton<INTERFACEDEFSTACK>()->push(std::vector<CSceneNodeInterfaceDef *>());
	using namespace boost::property_tree;
	//int n = tr.get<int>(L"Count");
	//m_children.reserve(n);
	for(wptree::const_iterator itr = tr.begin(); itr!=tr.end(); ++itr){
		ISceneNodeFactory *pFac = GetSingleton<CSceneNodeFactoryRegistry>()->GetFactory(itr->first);
		if(pFac){
			m_children.push_back(pFac->Create(itr->second));
		}else{
			assert(0);
			//assert(itr->first == L"Count");
		}
	}
	std::for_each(
		GetSingleton<INTERFACEDEFSTACK>()->top().begin(),
		GetSingleton<INTERFACEDEFSTACK>()->top().end(),
		std::mem_fun(&CSceneNodeInterfaceDef::CatchControlPlanes)
		);
	std::for_each(
		GetSingleton<INTERIORDEFSTACK>()->top().begin(),
		GetSingleton<INTERIORDEFSTACK>()->top().end(),
		std::mem_fun(&CSceneNodeInteriorDef::CatchVerts)
		);
	std::for_each(
		GetSingleton<WALLDEFSTACK>()->top().begin(),
		GetSingleton<WALLDEFSTACK>()->top().end(),
		std::mem_fun(&CSceneNodeWallDef::CatchVerts)
		);
	GetSingleton<INTERFACEDEFSTACK>()->pop();
	GetSingleton<INTERIORDEFSTACK>()->pop();
	GetSingleton<WALLDEFSTACK>()->pop();
	GetSingleton<CRegistryStack<CSceneNodePolygonModel *>>()->Pop();
	//assert(m_children.size() == n);
}


void CSceneNodeObjectSet::OnInsertion(){
	std::for_each(m_children.begin(),m_children.end(),std::mem_fun(&ISceneNode::OnInsertion)); 
}


CSceneNodeObjectSet::~CSceneNodeObjectSet(){
	for(std::vector<ISceneNode *>::iterator itr = m_children.begin(); itr!= m_children.end(); ++itr){
		delete *itr;
	}
}

void CSceneNodeObjectSet::EnlargeAABB(AABB &aabb)const{
	for(std::vector<ISceneNode *>::const_iterator itr = m_children.begin(); itr!= m_children.end(); ++itr){
		(*itr)->EnlargeAABB(aabb);
	}
}

void CSceneNodeObjectSet::UpdateTransform(const matrix44 &parentTransform){
	for(std::vector<ISceneNode *>::iterator itr(m_children.begin()); itr!=m_children.end(); ++itr){
		(*itr)->UpdateTransform(parentTransform);
	}
}