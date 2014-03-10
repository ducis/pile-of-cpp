#pragma once

#include "CTypedSceneNodeManager.h"
#include <cassert>

template <class SCENENODE>
CTypedSceneNodeManager<SCENENODE>::CTypedSceneNodeManager():m_pNodes(),m_itrCurrent(m_pNodes.end()){
}


template <class SCENENODE>
SCENENODE *CTypedSceneNodeManager<SCENENODE>::GetCurrent(){
	return m_pNodes.empty() ? NULL : *m_itrCurrent;
}


template <class SCENENODE>
void CTypedSceneNodeManager<SCENENODE>::Next(){
	if(m_pNodes.empty()){
		return;
	}
	++m_itrCurrent;
	if(m_pNodes.end() == m_itrCurrent){
		m_itrCurrent = m_pNodes.begin();
	}
}

template <class SCENENODE>
void CTypedSceneNodeManager<SCENENODE>::Prev(){
	if(m_pNodes.empty()){
		return;
	}
	if(m_pNodes.begin() == m_itrCurrent){
		m_itrCurrent = m_pNodes.end();
	}
	--m_itrCurrent;
}


template <class SCENENODE>
void CTypedSceneNodeManager<SCENENODE>::Register(SCENENODE *p){
	assert(p);
	m_itrCurrent = m_pNodes.insert(p).first;
}
