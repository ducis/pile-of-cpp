#pragma once

#include "ISceneNodeFactory.h"

#include <functional>

//template< std::unary_function< typename boost::property_tree::wptree &, ISceneNode * > Function >
template<typename RETURNTYPE, RETURNTYPE Function( const boost::property_tree::wptree & ) >
class CSceneNodeFactoryFunctionCall:public ISceneNodeFactory{
	//����ĳ����̬��ȫ�ֺ�������������Ĺ�����
public:
	ISceneNode* Create( const boost::property_tree::wptree &tr ){ return Function(tr); }
};
