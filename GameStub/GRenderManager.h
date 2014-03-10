#ifndef GRENDERMANAGER_H
#define GRENDERMANAGER_H
#include "index.h"
#include "D3D9.h"
#include "D3DX9.h"
#include <Common/SceneData/Light/hkxLight.h>
#include <Common/SceneData/Material/hkxMaterial.h>
#include <Common/SceneData/Camera/hkxCamera.h>
class GRenderManager{
private:
	struct HANDLES{
		D3DXHANDLE AmbientLight,
			DirectionalLightDiffuse,
			DirectionalLightSpecular,
			DirectionalLightVectorWS,
			CamPositionWS,
			MatrixInverseWorld,
			MatrixWorldViewProjection,
			MaterialDiffuse,
			MaterialSpecular,
			//MaterialAmbient,//MaterialAmbient==MaterialDiffuse
			MaterialEmissive,
			MatrixWorld;
		void Setup(LPD3DXEFFECT pEffect);
	}m_handles;
	void Camera2Matrix();
public:
	long				m_numLiveDirectionalLight;
	long long			m_lastLightCountChange;
	LPD3DXEFFECTPOOL	m_pEffectPool;
	LPD3DXEFFECT		m_pCurEffect;
	LPD3DXEFFECT		m_pParameterContainerEffect;
	long long			m_directionalLightID[MAX_DIRECTIONAL_LIGHT];
	D3DXVECTOR3			m_directionalLightVectorWS[MAX_DIRECTIONAL_LIGHT];
//	D3DXVECTOR3			m_bufDirectionalLightVectorWS[MAX_DIRECTIONAL_LIGHT];
	D3DXVECTOR3			m_directionalLightDiffuse[MAX_DIRECTIONAL_LIGHT];
	D3DXVECTOR3			m_directionalLightSpecular[MAX_DIRECTIONAL_LIGHT];
	long long			m_nextID;
	long				m_numLiveDirectionalLightSnapshot;
	D3DXVECTOR4			m_ambient;
	D3DXMATRIX			m_matrixWorld;
	D3DXMATRIX			m_matrixInverseWorld;
	D3DXMATRIX			m_matrixView;
	D3DXMATRIX			m_matrixProjection;
	D3DXMATRIX			m_matrixWorldViewProjection;
	D3DXVECTOR3			m_cameraPosition;
	D3DXVECTOR3			m_cameraLookAt;
	D3DXVECTOR3			m_cameraUp;
	float				m_cameraFOV;
	float				m_cameraNear;
	float				m_cameraFar;
	DWORD				m_state;
	float				m_aspectRatio;
	void BeginBatching();
	void EndBatching();
	long long AddLight(hkxLight *pLight);
	long long AddLight(D3DLIGHT9 *pLight);
	long long AddDirectionalLight(D3DXVECTOR3 direction,D3DXVECTOR3 color);
	long long AddDirectionalLight(D3DXVECTOR3 direction,D3DXVECTOR3 diffuse,D3DXVECTOR3 specular);
	long long AddDirectionalLightBasic(D3DXVECTOR3 vector,D3DXVECTOR3 diffuse,D3DXVECTOR3 specular);
	void SetAmbient(D3DXVECTOR3 ambient);
	void AddAmbient(D3DXVECTOR3 ambient);
	void KillLight(long id);
	void UpdateDirectionalLights();
	void UpdateAmbient();
	void SetWorldTransform(D3DXMATRIX *mat);
	void SetCamera(hkxCamera *pCamera);
	void UpdateCameraPosition();
	void UpdateMatrixWorldViewProjection();
	void ApplyMaterialColor(hkxMaterial *material);
	void Setup(LPDIRECT3DDEVICE9 pd3d9Device);
	GRenderManager();
};
#endif
