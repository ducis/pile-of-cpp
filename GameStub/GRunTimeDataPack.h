#ifndef GRUNTIMEDATAPACK_H
#define GRUNTIMEDATAPACK_H
#include <D3D9.h>
#include <D3DX9.h>
class GRunTimeDataPack{
public:
	long long m_lastLightCountChange;
	LPDIRECT3DSTATEBLOCK9 m_pStateBlock;
	virtual void force_polymorphic(){};
	GRunTimeDataPack():m_pStateBlock(NULL),m_lastLightCountChange(-0x7FFFFFFF){};
};
class GRunTimeDataPackWithEffect:public GRunTimeDataPack{
public:
	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_technique;
	D3DXHANDLE m_parameterBlock;
	GRunTimeDataPackWithEffect():GRunTimeDataPack(),m_pEffect(NULL),m_technique(NULL){}
};
class GRunTimeDataPackWithShader:public GRunTimeDataPack{
public:
	LPDIRECT3DVERTEXSHADER9 m_pVShader9;
	LPD3DXCONSTANTTABLE m_pVSConstantTable;
	LPDIRECT3DPIXELSHADER9 m_pPShader9;
	LPD3DXCONSTANTTABLE m_pPSConstantTable;
	GRunTimeDataPackWithShader():GRunTimeDataPack(),m_pVShader9(NULL),m_pVSConstantTable(NULL),m_pPShader9(NULL),m_pPSConstantTable(NULL){}
};
#endif
