#pragma once

#include "ISceneNodeFactory.h"

template<class SCENENODECLASS>
class CSceneNodeFactorySimple:public ISceneNodeFactory{
	//最简单的用xml结点作参数的工厂类,直接new
public:
	ISceneNode* Create( const boost::property_tree::wptree &tr ){ return new SCENENODECLASS(tr); }
	//ISceneNode* Create() { return new SCENENODECLASS; }
};
