#ifndef CGRAPHICS_H
#define CGRAPHICS_H
#include <windows.h>
#include <GL/gl.h>
#include <CG/cg.h>
#include "CNamedVarManager.h"
#include "TypeMacros.h"
#include <stack>
#include <vector>
#define GRAPHICS_MAX_OFFSCRRT 64
#define GRAPHICS_MAX_DRAWCALLARGS 4096
//#include <CEGUI.h>

class CGraphics{
public:
	struct STATES{
		unsigned int bLockRT:1;
		unsigned int bLockDRT:1;
		STATES():bLockRT(0),bLockDRT(0){}
	}m_states;
	//LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND						m_hWnd;
	class CVidConfig			*m_pVidConfig;
	class CFixedPipeShadow		*m_pFixedPipeShadow;
	class CLog					*m_pLog;
	HDC							m_hDC;
	HGLRC						m_hGLRC,m_hGLRC_comp;
	CGcontext					m_cgContext;
	class CResourceManager		*m_pRcMgr;
	union OFFSCREENRT{
		struct GL{
			unsigned int fbo;
			GLenum target;
			//x`GLenum attachment;
			GLenum textarget;
			unsigned int texture;
		}gl;
		struct D3D{
		}d3d;
	}m_offscreenRTPool[GRAPHICS_MAX_OFFSCRRT],m_offscreenDepthRTPool[GRAPHICS_MAX_OFFSCRRT];//ping-pong buffering is yet to be implemented.
	struct{
		unsigned int used:1;
	}m_offscreenRTState[GRAPHICS_MAX_OFFSCRRT],m_offscreenDepthRTState[GRAPHICS_MAX_OFFSCRRT];
	int m_sizeOffscreenRTPool,m_sizeOffscreenDepthRTPool;
	GLuint m_commonFBO;
	struct DRAWCALLARG{
		GLenum mode;
		GLsizei count;
		GLenum type;
		union{
			GLint first;
			const GLvoid *indices;
		}firstOrIndices;
	}m_drawCallArgs[GRAPHICS_MAX_DRAWCALLARGS];
	int m_numDrawCallArgs;
	unsigned int m_vboFullscreenQuad,m_vboFullscreenQuadUV;
	class CSASScriptRenderer	*m_pSASScriptRenderer;
	int	m_forcedPrimitiveType;
public:
	class CTimer *m_pTimer;
	CGparameter m_timeCgParam;
	//Viewport==>>
	CGparameter m_viewportSizeCgParam;
	void PushViewport(int x,int y,int w,int h);
	void PushViewport();
	void SetViewport(int x,int y,int w,int h);
	void PopViewport();
	//Lights==>>
#define	GRAPHICS_MAXLIGHTPERTYPE	 32
	VEC4 m_globalAmbient;
	CGparameter m_globalAmbientVec3CgParam;
	CGparameter m_globalAmbientVec4CgParam;
	struct LIGHT{
		enum{
			POSITION,
			DIRECTION,
			DIFFUSE,
			SPECULAR,
			AMBIENT,
			NUMVECS
		};
		VEC4 vecs[NUMVECS];
		CGparameter vec3CgParams[NUMVECS];
		CGparameter vec4CgParams[NUMVECS];
		enum{
			POWER,
			CONSTANTATTENUATION,
			LINEARATTENUATION,
			QUADRATICATTENUATION,
			FALLOFFANGLE,
			FALLOFFEXPONENT,
			NUMSCLS
		};
		enum{
			SPOTLIGHT,
			POINTLIGHT,
			DIRECTIONALLIGHT,
			NUMLIGHTTYPES
		};
		float scls[NUMSCLS];
		CGparameter sclCgParams[NUMSCLS];
		bool on;
	};
	LIGHT m_lights[LIGHT::NUMLIGHTTYPES][GRAPHICS_MAXLIGHTPERTYPE];
	std::vector<LIGHT *>	m_lightsUsedInFixedPipe;
	//Materials==>>
	std::stack<class CMaterial*> m_stackMaterial;
	//Transforms==>>
#define GRAPHICS_MAXMATTYPE 64
	CNamedVarManager<int,-1> m_matTypeIndexManager;
	enum MATRIXOP{
		MATRIXOP_WORLD,
		MATRIXOP_VIEW,
		MATRIXOP_PROJECTION,
		MATRIXOPNUMMATRIX,
		MATRIXOP_INVERSE,
		MATRIXOP_TRANSPOSE,
		MATRIXOPCOUNT
	};
	struct MATRIXTYPE{
		int mat,op;
		int affected[GRAPHICS_MAXMATTYPE];
		int nAffected;
		MATRIXTYPE():mat(-1),op(-1),nAffected(0){}
	}m_matTypes[GRAPHICS_MAXMATTYPE];
	int m_nMatType;
	int m_matUsage[GRAPHICS_MAXMATTYPE];
	bool m_bMatChanged[GRAPHICS_MAXMATTYPE];
	//bool m_bMatUsed[GRAPHICS_MAXMATTYPE];
	MATRIX44 m_mats[GRAPHICS_MAXMATTYPE];
	CGparameter m_sharedCgMatrixParameter[GRAPHICS_MAXMATTYPE];
	bool m_bWorldTransformChanged,m_bViewTransformChanged,m_bProjTransformChanged;
#define GRAPHICS_MATSTACKSIZE 1024
	MATRIX44 m_worldTransformStack[GRAPHICS_MATSTACKSIZE];
	MATRIX44 m_viewTransformStack[GRAPHICS_MATSTACKSIZE];
	MATRIX44 m_projTransformStack[GRAPHICS_MATSTACKSIZE];
	MATRIX44 *m_pTopWorldTransform;
	MATRIX44 *m_pTopViewTransform;
	MATRIX44 *m_pTopProjTransform;
	bool m_bWorldTransformStackChanged[GRAPHICS_MATSTACKSIZE];
	bool m_bViewTransformStackChanged[GRAPHICS_MATSTACKSIZE];
	bool m_bProjTransformStackChanged[GRAPHICS_MATSTACKSIZE];

	DWORD Init(HWND hWnd, class CVidConfig *pVidConfig);
	DWORD InitGLForWindows();
	DWORD InitCgCore();
	DWORD InitCgGL();
	void InitMatrixManagement();
	DWORD SetPixelFormatGL(); 
	DWORD BeginScene();
	DWORD EndScene();
	void InitScriptedEffectSystem();
	void InvokeDrawCall();
	void DrawFullscreenQuad();
	void InitFullscreenQuadVBO();

	void ClearDrawCalls();
	void SetSolitaryDrawCall(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices);
	void SetSolitaryDrawCall(GLenum mode,GLint first,GLsizei count);
	void AddDrawCall(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices);
	void AddDrawCall(GLenum mode,GLint first,GLsizei count);
	//Resource management==>>
	//CScriptedEffect *LoadScriptedEffect(const char *fname);
	class CScriptedEffect *UseScriptedEffect(const char *fname);
	class CScriptedEffect *GetScriptedEffect(const char *fname);
	void PopScriptedEffect();
	void ClearScriptedEffect();
	//class CMaterial *LoadMaterial(const char *fname);
	class CMaterial *GetMaterial(const char *fname);
	//TEXTUREHANDLE LoadTexture(const char *fname);
	TEXTUREHANDLE GetTexture(const char *fname);
	TEXTUREHANDLE GetTexture(const wchar_t *fname);

	//Render target management==>>
	OFFSCREENRT* AcquireOffscreenRT();
	void UseOffscreenRT(OFFSCREENRT::GL *pRt,int id);
	void ReleaseOffscreenRT(OFFSCREENRT *pRt);
	DWORD IncreaseOffscreenRTPoolSize();
	OFFSCREENRT* AcquireOffscreenDepthRT();
	void UseOffscreenDepthRT(OFFSCREENRT::GL *pRt);
	void ReleaseOffscreenDepthRT(OFFSCREENRT *pRt);
	DWORD IncreaseOffscreenDepthRTPoolSize();

	//Transforms==>>
	void PushWorldTransform();
	void PushWorldTransform(MATRIX44 *pMat);
	void PopWorldTransform();
	void SetWorldTransform(MATRIX44 *pMat);
	void MultWorldTransformObjectSpace(MATRIX44 *pMat);

	void PushViewTransform();
	void PushViewTransform(MATRIX44 *pMat);
	void PopViewTransform();
	void SetViewTransform(MATRIX44 *pMat);
	MATRIX44 *GetViewTransform();
	//void MultViewTransform();

	void PushProjTransform();
	void PushProjTransform(MATRIX44 *pMat);
	void PopProjTransform();
	void SetProjTransform(MATRIX44 *pMat);

	int GetMatTypeIndex(const char *namerev);
	void UploadMatrices();
	void ReloadAllMatrices();
	void MarkAllTransformAsChanged();
	void ReloadProjMatrixGL();

	//Lights
	LIGHT *GetUnusedLight(int lighttype);
	void UploadShaderLight(LIGHT *pLight);
	void UploadFixedPipeLights();
	void InitLights();
	void SetGlobalAmbient(const VEC4 &ambient);
	void UploadLightVector(LIGHT *pLight,int id);
	void UploadLightScalar(LIGHT *pLight,int id);
	void SetLightParam(LIGHT *pLight,int id,VEC4 *pVec);
	void SetLightParam(LIGHT *pLight,int id,float f);
	void UseLightInFixedPipe(LIGHT *pLight);
	//Materials	
	void PushMaterial(CMaterial* pMtl);
	void PopMaterial();
	void RenderWithCurrentMaterial();
	
	void LockRTDRT();
	void UnlockRTDRT();
	//class CScriptedEffect *m_pForcedScriptedEffect;
	//int m_pForcedScriptedEffectCallCounter;
	//void BeginForcedScriptedEffect(class CScriptedEffect *pScriptedEffect);
	//void EndForcedScriptedEffect();

	CGraphics();
};
#endif
