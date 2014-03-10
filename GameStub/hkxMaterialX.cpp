#include "DXUT.h"
#include "index.h"
#include "hkxMaterialX.h"
#include <Common/SceneData/Material/hkxTextureFile.h>
#include <Common/SceneData/Attributes/hkxAttributeGroup.h>
#include <Common/SceneData/Attributes/hkxAttribute.h>
#include "typeinfo.h"
#include "GRunTimeDataPack.h"
#include "MyMaterialExtra.h"
#include "GNamedVarManager.h"
#include "GEffectGroup.h"
#include "stdio.h"
#include "io.h"
#include <D3DX9.h>
#include "GRenderManager.h"
//extern GNamedVarManager<LPDIRECT3DTEXTURE9,NULL> TexFileManager;
GNamedVarManager<LPDIRECT3DTEXTURE9,NULL> g_TexFileManager;
GNamedVarManager<GEffectGroup*,NULL> g_EffectFileManager;
GNamedVarManager<void (hkxMaterialX::*)(const hkxAttribute *,float),&hkxMaterialX::ReportUnusedAttribute,NULL,hkxMaterialX::AttributeNameList,hkxMaterialX::AttributeParserList> hkxMaterialX::AttributeParserManager;
extern LPDIRECT3DDEVICE9 g_pCurDevice;
extern GRenderManager g_renderManager;
void hkxMaterialX::SetDeviceState(){
	//MyMaterialExtra *extra=reinterpret_cast<MyMaterialExtra*>(m_extraData.m_object);
	if(((MyMaterialExtra*)m_extraData.val())->m_pRunTime){
		((MyMaterialExtra*)m_extraData.val())->m_pRunTime->m_pStateBlock->Apply();
		if(typeid(*((MyMaterialExtra*)m_extraData.val())->m_pRunTime)==typeid(GRunTimeDataPackWithEffect)){
			GRunTimeDataPackWithEffect *runtime=((GRunTimeDataPackWithEffect*)((MyMaterialExtra*)m_extraData.val())->m_pRunTime);
			HRESULT r=runtime->m_pEffect->ApplyParameterBlock(runtime->m_parameterBlock);
			assert(r==D3D_OK);
		}
	}else{
		RecordDeviceState();
	}
}
//extern LPD3DXCONSTANTTABLE pVSConstantTable;
//extern LPD3DXCONSTANTTABLE pPSConstantTable;
void hkxMaterialX::RecordDeviceState(){	
	g_pCurDevice->BeginStateBlock();
	const hkxAttributeGroup *group=findAttributeGroupByName("main");
	for(long i=0;i<group->m_attributes.getSize();++i){
		(this->*(AttributeParserManager.GetVar(group->m_attributes[i].m_name)))(&group->m_attributes[i],0);
	}
	if(!((MyMaterialExtra*)m_extraData.val())->m_pRunTime){
		((MyMaterialExtra*)m_extraData.val())->m_pRunTime=new GRunTimeDataPack;
	}
//	const char *a=typeid(*((MyMaterialExtra*)m_extraData.m_object)->m_pRunTime).name();
	if(typeid(*((MyMaterialExtra*)m_extraData.val())->m_pRunTime)==typeid(GRunTimeDataPackWithEffect)){
		GRunTimeDataPackWithEffect *runtime=(GRunTimeDataPackWithEffect *)((MyMaterialExtra*)m_extraData.val())->m_pRunTime;
		runtime->m_pEffect->BeginParameterBlock();
		g_renderManager.m_pCurEffect=runtime->m_pEffect;
		g_renderManager.ApplyMaterialColor(this);
	}else{
		assert(false);
	}
	for(long i=0;i<8;++i){
		g_pCurDevice->SetTexture(i,NULL);
	}
	for(long i=0;i<m_stages.getSize();++i){
		LPDIRECT3DTEXTURE9 pTex=NULL;
		if(m_stages[i].m_texture.getClass()==&hkxTextureFileClass){
			hkxTextureFile* hTex=(hkxTextureFile*)m_stages[i].m_texture.val();
			LPDIRECT3DTEXTURE9 *ppTex=g_TexFileManager.GetPtrReady(hTex->m_filename);
			if(*ppTex==NULL){
				D3DXCreateTextureFromFileExA(g_pCurDevice,hTex->m_filename,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_FILTER_BOX,0,NULL,NULL,ppTex);
			}
			pTex=*ppTex;
		}
		if(typeid(*((MyMaterialExtra*)m_extraData.val())->m_pRunTime)==typeid(GRunTimeDataPackWithEffect)){
			LPD3DXEFFECT effect=((GRunTimeDataPackWithEffect*)((MyMaterialExtra*)m_extraData.val())->m_pRunTime)->m_pEffect;
			D3DXHANDLE handle=NULL;
			if(m_stages[i].m_usageHint==TEX_DIFFUSE){
				static D3DXHANDLE h=NULL;
				if(!h){
					h=effect->GetParameterByName(NULL,"sDiffuse");
				}
				handle=h;
			}else if(m_stages[i].m_usageHint==TEX_SPECULAR){
				static D3DXHANDLE h=NULL;
				if(!h){
					h=effect->GetParameterByName(NULL,"sDiffuse");
				}
				handle=h;
			}else if(m_stages[i].m_usageHint==TEX_NORMAL){
				static D3DXHANDLE h=NULL;
				if(!h){
					h=effect->GetParameterByName(NULL,"sNormal");
				}
				handle=h;
			}
			effect->SetTexture(handle,pTex);
		}else{
			assert(false);
			long st=i;
			g_pCurDevice->SetTexture(st,pTex);
			g_pCurDevice->SetTextureStageState(st,D3DTSS_TEXCOORDINDEX ,m_stages[i].m_tcoordChannel);
		}
	}
	g_pCurDevice->EndStateBlock( &(reinterpret_cast<MyMaterialExtra*>(m_extraData.val()))->m_pRunTime->m_pStateBlock);
	if(typeid(*((MyMaterialExtra*)m_extraData.val())->m_pRunTime)==typeid(GRunTimeDataPackWithEffect)){
		GRunTimeDataPackWithEffect *runtime=(GRunTimeDataPackWithEffect *)((MyMaterialExtra*)m_extraData.val())->m_pRunTime;
		runtime->m_parameterBlock=runtime->m_pEffect->EndParameterBlock();
		runtime->m_pEffect->ApplyParameterBlock(runtime->m_parameterBlock);
	}else{
		assert(false);
	}
}
void hkxMaterialX::ReportUnusedAttribute(const hkxAttribute *attribute,float time){
}
char *hkxMaterialX::AttributeNameList[]={
	"UseFX",
	NULL
};
void (hkxMaterialX::*hkxMaterialX::AttributeParserList[])(const hkxAttribute *,float)={
	&hkxMaterialX::ParseAttribute_UseFX
};
long Time2FrameId(float time,float *timeseq,long num);
void hkxMaterialX::ParseAttribute_UseFX(const hkxAttribute *attribute,float time){
	MyMaterialExtra *pMatEx=(MyMaterialExtra*)m_extraData.val();
	if(pMatEx->m_pRunTime==NULL){
		pMatEx->m_pRunTime=new GRunTimeDataPackWithEffect;
	}else{
		assert(typeid(*pMatEx->m_pRunTime)==typeid(GRunTimeDataPackWithEffect));
	}
	const char* fname=NULL;
	if(attribute->m_value.getClass()==&hkxSparselyAnimatedStringClass){
		hkxSparselyAnimatedString *sas=reinterpret_cast<hkxSparselyAnimatedString *>(attribute->m_value.val());
		long frame=Time2FrameId(time,sas->m_times.begin(),sas->m_times.getSize());
		fname=sas->m_strings[frame].cString();
	}
	assert(fname);
	GEffectGroup **ppEffectGroup=g_EffectFileManager.GetPtrReady(fname);

	if(*ppEffectGroup==NULL){
		extern const long g_utilityBufferLength;
		extern char g_utilityBuffer[];
		FILE *f=fopen(fname,"rb");
		long flen=_filelength(_fileno(f));
		assert(flen>0);
		assert(flen<g_utilityBufferLength);
		static char *buf=g_utilityBuffer;
		fread(buf,1,flen,f);
		fclose(f);
		bool bMultipleBuffers=(strstr(buf,"NUM_DIRECTIONAL")!=NULL);
		if(bMultipleBuffers){
			*ppEffectGroup=new GEffectGroup_Multiple;
		}else{
			*ppEffectGroup=new GEffectGroup_Single;
		}		
		char tmp[8]="";
		D3DXMACRO Consts[]={
			{"NUM_DIRECTIONAL",tmp},
			{NULL,NULL}
		};
		long n=bMultipleBuffers?MAX_DIRECTIONAL_LIGHT:1;
		for(long i=0;i<n;++i){
			if(bMultipleBuffers){
				_itoa(i,(CHAR *)Consts[0].Definition,10);
//				char ttt[10];
//				_itoa(10,(CHAR *)0x00d22b14,10);
			}
			LPD3DXBUFFER pErrorMessage=NULL;
			assert(g_renderManager.m_pEffectPool);
			HRESULT hr=D3DXCreateEffect(g_pCurDevice,buf,flen,bMultipleBuffers?Consts:NULL,NULL,
				D3DXFX_NOT_CLONEABLE|D3DXSHADER_OPTIMIZATION_LEVEL3|D3DXSHADER_AVOID_FLOW_CONTROL|D3DXFX_DONOTSAVESTATE,g_renderManager.m_pEffectPool,
				&(bMultipleBuffers?
				dynamic_cast<GEffectGroup_Multiple*>(*ppEffectGroup)->effects[i]:dynamic_cast<GEffectGroup_Single*>(*ppEffectGroup)->effect),
				&pErrorMessage);
			/*HRESULT hr=D3DXCreateEffectFromFileA(g_pCurDevice,fname,bMultipleBuffers?Consts:NULL,NULL,
				D3DXSHADER_SKIPOPTIMIZATION |D3DXSHADER_DEBUG ,g_renderManager.m_pEffectPool,
				&(bMultipleBuffers?
				dynamic_cast<GEffectGroup_Multiple*>(*ppEffectGroup)->effects[i]:dynamic_cast<GEffectGroup_Single*>(*ppEffectGroup)->effect),
				&pErrorMessage);*/

			if(D3D_OK!=hr){
				FILE *f=fopen("error.txt","wb");
				fwrite(pErrorMessage->GetBufferPointer(),1,pErrorMessage->GetBufferSize(),f);
				fclose(f);
				MessageBoxA(NULL,fname,"Cannot compile effect",0);
				exit(10);
			}else if(pErrorMessage){
				FILE *f=fopen("EffectWarning.txt","wb");
				fwrite(pErrorMessage->GetBufferPointer(),1,pErrorMessage->GetBufferSize(),f);
				fclose(f);
			}
			if(pErrorMessage)
				pErrorMessage->Release();
		}
	}
	static_cast<GRunTimeDataPackWithEffect*>(pMatEx->m_pRunTime)->m_pEffect=(*ppEffectGroup)->GetCurrentEffect();
}
