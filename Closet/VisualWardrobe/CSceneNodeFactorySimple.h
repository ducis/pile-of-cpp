#pragma once

#include "ISceneNodeFactory.h"

template<class SCENENODECLASS>
class CSceneNodeFactorySimple:public ISceneNodeFactory{
	//��򵥵���xml����������Ĺ�����,ֱ��new
public:
	ISceneNode* Create( const boost::property_tree::wptree &tr ){ return new SCENENODECLASS(tr); }
	//ISceneNode* Create() { return new SCENENODECLASS; }
};
