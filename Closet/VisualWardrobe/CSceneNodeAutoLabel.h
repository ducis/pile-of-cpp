#pragma once

#include "ISceneNode.h"
#include "ITransformBaked.h"
#include "IFloatingSymbol.h"
#include "ptree.h"
#include "mtxlib.h"
#include <map>

class CSceneNodeAutoLabel:public ISceneNode,public ITransformBaked,public IFloatingSymbol{
public:
	//显示自动标号的类
	void Render() const{}
	virtual void RenderSymbol();
	void OnInsertion();
	void BakeTransform(int x_mm,int y_mm,int z_mm,int rotZ_90deg);
	CSceneNodeAutoLabel(const boost::property_tree::wptree &tr);
public:
	vector3 m_pos;
	std::wstring m_labelCategory;
	std::string m_labelStr;

	typedef std::map<std::wstring,unsigned int> COUNTERS;
	static COUNTERS s_counters;
	typedef std::map<std::wstring,std::wstring> DEFS;
	static DEFS s_defs;
};
