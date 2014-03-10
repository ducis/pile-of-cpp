#include "DXUT.h"
#include "GRenderManager.h"
#include "D3DX9.h"
#include <stdio.h>
GRenderManager g_renderManager;
GRenderManager::GRenderManager():
								m_numLiveDirectionalLight(0),
								m_lastLightCountChange(0),
								m_pCurEffect(NULL),
								m_numLiveDirectionalLightSnapshot(-1),
								m_nextID(0),
								m_ambient(0.0f,0.0f,0.0f,0.0f),
								m_aspectRatio(16.0f/9.0f)
{
	D3DXCreateEffectPool(&m_pEffectPool);
	memset(m_directionalLightID,0xFF,sizeof(m_directionalLightID));
	D3DXMatrixIdentity(&m_matrixWorld);
	memset(&m_handles,0,sizeof(m_handles));
	m_matrixInverseWorld = 
		m_matrixWorldViewProjection = 
		m_matrixProjection = 
		m_matrixView = 
		m_matrixWorld;
}
#define BATCHING (m_numLiveDirectionalLightSnapshot>=0)
#define NOT_BATCHING (m_numLiveDirectionalLightSnapshot<0)
void GRenderManager::BeginBatching(){
	if(NOT_BATCHING)
		m_numLiveDirectionalLightSnapshot=m_numLiveDirectionalLight;
}
void GRenderManager::EndBatching(){
	if(BATCHING){
		if(m_numLiveDirectionalLightSnapshot!=m_numLiveDirectionalLight){
			++m_lastLightCountChange;
			UpdateDirectionalLights();
		}
	}
}
long long GRenderManager::AddLight(hkxLight *pLight){
	assert(pLight);
	if(pLight->m_type==hkxLight::DIRECTIONAL_LIGHT){
		const hkQuadReal &q=pLight->m_direction.getQuad();
		//const hkQuadReal &c=pLight->m_color.getQuad();
		D3DXVECTOR3 c((((pLight->m_color>>16)&0xFF)*(1.0f/255.0f)),(((pLight->m_color>>8)&0xFF)*(1.0f/255.0f)),(((pLight->m_color)&0xFF)*(1.0f/255.0f)));
		return AddDirectionalLightBasic(D3DXVECTOR3(-q.x,-q.y,-q.z),c,c);
	}else{
		assert(false);
		return -1;
	}
}
long long GRenderManager::AddLight(D3DLIGHT9 *pLight){
	assert(pLight);
	if(pLight->Type==D3DLIGHT_DIRECTIONAL){
		return AddDirectionalLightBasic(D3DXVECTOR3(-pLight->Direction.x,-pLight->Direction.y,-pLight->Direction.z),
											D3DXVECTOR3(pLight->Diffuse.r,pLight->Diffuse.g,pLight->Diffuse.b),
											D3DXVECTOR3(pLight->Specular.r,pLight->Specular.g,pLight->Specular.b));
	}else{
		assert(false);
		return -1;
	}
}
long long GRenderManager::AddDirectionalLightBasic(D3DXVECTOR3 vector, D3DXVECTOR3 diffuse, D3DXVECTOR3 specular){
	assert(m_numLiveDirectionalLight<MAX_DIRECTIONAL_LIGHT);
	if(m_numLiveDirectionalLight>=MAX_DIRECTIONAL_LIGHT){
		return -1;
	}
	long long id=m_nextID++;
	long pos=0;
	while(m_directionalLightID[pos]>=0){
		++pos;
		assert(pos<MAX_DIRECTIONAL_LIGHT);
	}
	m_directionalLightID[pos]=id;
	m_directionalLightVectorWS[pos]=vector;
	m_directionalLightDiffuse[pos]=diffuse;
	m_directionalLightSpecular[pos]=specular;
	++m_numLiveDirectionalLight;
	if(NOT_BATCHING){
		++m_lastLightCountChange;
		UpdateDirectionalLights();
	}
	return id;
}
long long GRenderManager::AddDirectionalLight(D3DXVECTOR3 direction, D3DXVECTOR3 color){
	return AddDirectionalLightBasic(-direction,color,color);
}
long long GRenderManager::AddDirectionalLight(D3DXVECTOR3 direction, D3DXVECTOR3 diffuse, D3DXVECTOR3 specular){
	return AddDirectionalLightBasic(-direction,diffuse,specular);
}
void GRenderManager::AddAmbient(D3DXVECTOR3 ambient){
	m_ambient+=D3DXVECTOR4(ambient,0.0f);
	UpdateAmbient();
}
void GRenderManager::SetAmbient(D3DXVECTOR3 ambient){
	m_ambient=D3DXVECTOR4(ambient,0.0f);
	UpdateAmbient();
}
void GRenderManager::UpdateAmbient(){
	//static D3DXHANDLE handle=NULL;
	if(!m_handles.AmbientLight){
		m_handles.AmbientLight=m_pCurEffect->GetParameterByName(NULL,"AmbientLight");
	}
	m_pCurEffect->SetVector(m_handles.AmbientLight,&m_ambient);
}
void GRenderManager::KillLight(long id){
	long pos=0;
	while(m_directionalLightID[pos]!=id&&pos<MAX_DIRECTIONAL_LIGHT){
		++pos;
	}
	if(pos<MAX_DIRECTIONAL_LIGHT){
		m_directionalLightID[pos]=-1;
		--m_numLiveDirectionalLight;
		if(NOT_BATCHING){
			++m_lastLightCountChange;
			UpdateDirectionalLights();
		}
	}
}
void GRenderManager::UpdateDirectionalLights(){
	long t=0;
	D3DXVECTOR4 bufVectorWS[MAX_DIRECTIONAL_LIGHT],bufDiffuse[MAX_DIRECTIONAL_LIGHT],bufSpecular[MAX_DIRECTIONAL_LIGHT];
	for(long i=0;i<MAX_DIRECTIONAL_LIGHT;++i){
		if(m_directionalLightID[i]>=0){
			bufVectorWS[t]=D3DXVECTOR4(m_directionalLightVectorWS[i],0.0f);
			bufDiffuse[t]=D3DXVECTOR4(m_directionalLightDiffuse[i],0.0f);
			bufSpecular[t]=D3DXVECTOR4(m_directionalLightSpecular[i],0.0f);
			++t;
		}
	}
	assert(t==m_numLiveDirectionalLight);
	assert(m_handles.DirectionalLightVectorWS);
	assert(m_handles.DirectionalLightDiffuse);
	assert(m_handles.DirectionalLightSpecular);
	m_pCurEffect->SetVectorArray(m_handles.DirectionalLightVectorWS,bufVectorWS,t);
	m_pCurEffect->SetVectorArray(m_handles.DirectionalLightDiffuse,bufDiffuse,t);
	m_pCurEffect->SetVectorArray(m_handles.DirectionalLightSpecular,bufSpecular,t);
}
void GRenderManager::SetWorldTransform(D3DXMATRIX *pMat){
	m_matrixWorld=*pMat;
	D3DXMatrixInverse(&m_matrixInverseWorld,NULL,&m_matrixWorld);
	assert(m_handles.MatrixInverseWorld);
	m_pCurEffect->SetMatrix(m_handles.MatrixInverseWorld,&m_matrixInverseWorld);
	assert(m_handles.MatrixWorld);
	m_pCurEffect->SetMatrix(m_handles.MatrixWorld,&m_matrixWorld);
	m_matrixWorldViewProjection=m_matrixWorld*m_matrixView*m_matrixProjection;
	UpdateMatrixWorldViewProjection();
}
void GRenderManager::SetCamera(hkxCamera *pCamera){
	pCamera->m_from.store3(&m_cameraPosition.x);
	D3DXVECTOR4 camPositionWS(m_cameraPosition,1.0f);
	assert(m_handles.CamPositionWS);
	m_pCurEffect->SetVector(m_handles.CamPositionWS,&camPositionWS);
	pCamera->m_focus.store3(&m_cameraLookAt.x);
	pCamera->m_up.store3(&m_cameraUp.x);
	m_cameraFOV=pCamera->m_fov;
	m_cameraNear=pCamera->m_near;
	m_cameraFar=pCamera->m_far;
	Camera2Matrix();
	UpdateMatrixWorldViewProjection();
}
void GRenderManager::Camera2Matrix(){
	D3DXMatrixLookAtLH(&m_matrixView,&m_cameraPosition,&m_cameraLookAt,&m_cameraUp);
	D3DXMatrixPerspectiveFovLH(&m_matrixProjection,m_cameraFOV,m_aspectRatio,m_cameraNear,m_cameraFar);
	m_matrixWorldViewProjection=m_matrixWorld*m_matrixView*m_matrixProjection;
}
void GRenderManager::UpdateMatrixWorldViewProjection(){
	assert(m_handles.MatrixWorldViewProjection);
	m_pCurEffect->SetMatrix(m_handles.MatrixWorldViewProjection,&m_matrixWorldViewProjection);
}
void GRenderManager::ApplyMaterialColor(hkxMaterial *material){
	assert(m_handles.MaterialDiffuse);
	assert(m_handles.MaterialSpecular);
//	assert(m_handles.MaterialAmbient);
	assert(m_handles.MaterialEmissive);
	HRESULT r;
	r=m_pCurEffect->SetVector(m_handles.MaterialDiffuse,(D3DXVECTOR4 *)&material->m_diffuseColor);
	assert(r==D3D_OK);
	r=m_pCurEffect->SetVector(m_handles.MaterialSpecular,(D3DXVECTOR4 *)&material->m_specularColor);
	assert(r==D3D_OK);
//	m_pCurEffect->SetVector(m_handles.MaterialAmbient,(D3DXVECTOR4 *)&material->m_ambientColor);
	r=m_pCurEffect->SetVector(m_handles.MaterialEmissive,(D3DXVECTOR4 *)&material->m_emissiveColor);
	assert(r==D3D_OK);
//		pVSConstantTable->SetVector(g_pCurDevice,"DiffuseMaterial",(D3DXVECTOR4 *)&m_diffuseColor);
//		pVSConstantTable->SetVector(g_pCurDevice,"SpecularMaterial",(D3DXVECTOR4 *)&m_specularColor);
//		pPSConstantTable->SetVector(g_pCurDevice,"DiffuseMaterial",(D3DXVECTOR4 *)&m_diffuseColor);
//		pPSConstantTable->SetVector(g_pCurDevice,"SpecularMaterial",(D3DXVECTOR4 *)&m_specularColor);
}
void GRenderManager::Setup(LPDIRECT3DDEVICE9 pd3d9Device){
	HRESULT r=D3DXCreateEffectPool(&m_pEffectPool);
	assert(r==D3D_OK);
	LPD3DXBUFFER pErrors=NULL;
	//r=D3DXCreateEffectFromFileA(pd3d9Device,"D:\\DirectXSamples\\BasicHLSL10\\BasicHLSL.fx",NULL,NULL,D3DXSHADER_AVOID_FLOW_CONTROL|D3DXSHADER_OPTIMIZATION_LEVEL3|D3DXFX_DONOTSAVESTATE|D3DXFX_NOT_CLONEABLE,m_pEffectPool,&m_pParameterContainerEffect,&pErrors);
	r=D3DXCreateEffectFromFileA(pd3d9Device,"ParameterContainer.fx",NULL,NULL,D3DXSHADER_AVOID_FLOW_CONTROL|D3DXSHADER_OPTIMIZATION_LEVEL3|D3DXFX_DONOTSAVESTATE|D3DXFX_NOT_CLONEABLE,m_pEffectPool,&m_pParameterContainerEffect,&pErrors);
	if(pErrors){
		FILE *f=fopen("Error_ParameterContainer.fx.txt","wb");
		fwrite(pErrors->GetBufferPointer(),1,pErrors->GetBufferSize(),f);
		fclose(f);
		pErrors->Release();
	}
	assert(r==D3D_OK);
	m_pCurEffect=m_pParameterContainerEffect;
	m_handles.Setup(m_pCurEffect);
}
void GRenderManager::HANDLES::Setup(LPD3DXEFFECT pEffect){
	AmbientLight=pEffect->GetParameterByName(NULL,"AmbientLight");
	DirectionalLightDiffuse=pEffect->GetParameterByName(NULL,"DiffuseDirectional");
	DirectionalLightSpecular=pEffect->GetParameterByName(NULL,"SpecularDirectional");
	DirectionalLightVectorWS=pEffect->GetParameterByName(NULL,"LightVectorWSDirectional");
	CamPositionWS=pEffect->GetParameterByName(NULL,"CamPositionWS");
	MatrixInverseWorld=pEffect->GetParameterByName(NULL,"mInverseWorld");
	MatrixWorldViewProjection=pEffect->GetParameterByName(NULL,"mWorldViewProj");
	MaterialDiffuse=pEffect->GetParameterByName(NULL,"DiffuseMaterial");
	MaterialSpecular=pEffect->GetParameterByName(NULL,"SpecularMaterial");
//	MaterialAmbient=pEffect->GetParameterByName(NULL,"AmbientMaterial");
	MaterialEmissive=pEffect->GetParameterByName(NULL,"EmissiveMaterial");
	MatrixWorld=pEffect->GetParameterByName(NULL,"mWorld");
}
