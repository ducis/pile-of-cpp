#pragma once

#include "CRegistryStack.h"
#include <utility>


template <typename TYPE,typename KEY >
void CRegistryStack<TYPE,KEY>::Push(){
	m_stack.push(MAP());
}

template <typename TYPE,typename KEY >
void CRegistryStack<TYPE,KEY>::Pop(){
	m_stack.pop();
}

template <typename TYPE,typename KEY >
void CRegistryStack<TYPE,KEY>::Insert(KEY key, TYPE val){
	m_stack.top().insert(std::make_pair(key,val));
}

template <typename TYPE,typename KEY >
TYPE CRegistryStack<TYPE,KEY>::Find(KEY key, TYPE notFound){
	MAP::iterator itr = m_stack.top().find(key);
	if( m_stack.top().end() == itr ){
		return notFound;
	}else{
		return itr->second;
	}
}

template <typename TYPE,typename KEY >
CRegistryStack<TYPE,KEY>::CRegistryStack(){
	Push();
}

