#include "CClassicDrillingRule.h"

#include "singleton.h"
#include "CProgram.h"

void CClassicDrillingRule::Generate(float dim){
	m_holes.clear();
	if(dim>100){
		IHoleStyle *p(GetSingleton<CProgram>()->GetHoleStyle(L"ÈýºÏÒ»¿×"));
		assert(p);
		m_holes.push_back(std::make_pair(-32.0f,p));
		m_holes.push_back(std::make_pair(32.0f,p));
	}
	if(dim>180){
		IHoleStyle *p(GetSingleton<CProgram>()->GetHoleStyle(L"Ä¾é¾¿×"));
		assert(p);
		m_holes.push_back(std::make_pair(-64.0f,p));
		m_holes.push_back(std::make_pair(64.0f,p));
	}
}
