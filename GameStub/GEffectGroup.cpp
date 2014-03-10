#include "DXUT.h"
#include "GEffectGroup.h"
#include "d3d9.h"
#include "GRenderManager.h"
LPD3DXEFFECT GEffectGroup_Single::GetCurrentEffect(){
	return effect;
}
//extern LPDIRECT3DDEVICE9 g_pCurDevice;
extern GRenderManager g_renderManager;
LPD3DXEFFECT GEffectGroup_Multiple::GetCurrentEffect(){
	return effects[g_renderManager.m_numLiveDirectionalLight];
}
