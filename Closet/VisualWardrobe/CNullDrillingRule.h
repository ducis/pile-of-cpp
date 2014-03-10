#pragma once

#include "IDrillingRule.h"

class CNullDrillingRule: public IDrillingRule{//���ڲ��Ե���׹������Ƿ��ز����
public:
	virtual void Generate(float dim) {}
	virtual unsigned int GetHoleCount() const { return 0; }
	virtual class IHoleStyle *GetHoleStyle(unsigned int i) const { return NULL; }
	virtual float GetHolePos(unsigned int i) const { return 0.0f; }
};
