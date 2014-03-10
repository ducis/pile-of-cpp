#ifndef GEFFECTGROUP_H
#define GEFFECTGROUP_H
#include "index.h"
class GEffectGroup{
public:
	virtual LPD3DXEFFECT GetCurrentEffect()=0;
};
class GEffectGroup_Multiple:public GEffectGroup{
public:
	LPD3DXEFFECT effects[MAX_DIRECTIONAL_LIGHT];
	virtual LPD3DXEFFECT GetCurrentEffect();
};
class GEffectGroup_Single:public GEffectGroup{
public:
	LPD3DXEFFECT effect;
	virtual LPD3DXEFFECT GetCurrentEffect();
};
#endif
