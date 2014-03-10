#pragma once

#include "IDrillingRule.h"
#include <utility>
#include <vector>

class CClassicDrillingRule: public IDrillingRule{//一种测试用的钻孔规则
public:
	virtual void Generate(float dim);
	virtual unsigned int GetHoleCount() const{ return m_holes.size(); }
	virtual class IHoleStyle *GetHoleStyle(unsigned int i) const{ return m_holes[i].second; }
	virtual float GetHolePos(unsigned int i) const{ return m_holes[i].first; }
	//CClassicDrillingRule(){}
private:
	std::vector<std::pair<float,class IHoleStyle*> > m_holes;
};
