#include "DXUT.h"
#include "resource.h"
#include <cstdlib>
#include "index.h"
#include <Common/Base/keycode.cxx>
#include <Common/Base/KeyCode.h>
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

// Register versioning information.
//#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
//#include <Common/Compat/hkCompat_All.cxx> 

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
//#include <Common/Serialize/Packfile/hkPackfileReader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/SceneData/Scene/hkxScene.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>// Register Havok classes.
#include "GRenderManager.h"
#include "D:\MyDemo\Common\SerializedObjects.h"
#include "GActor.h"
long g_iScreenWidth=1366,g_iScreenHeight=768;
bool g_bWindowed=true;
hkpWorld* g_physicsWorld=NULL;
extern LPDIRECT3DDEVICE9 g_pCurDevice=NULL;
extern LPD3DXMATRIXSTACK g_pMatrixStack=NULL;
extern const long g_utilityBufferLength=1024*1024;
extern GRenderManager g_renderManager;
char g_utilityBuffer[g_utilityBufferLength];
static void HavokErrorToLog(const char* str, void* errorOutputObject)
{
	FILE *f=fopen("HavokError.log","a");
    fprintf(f,"%s", str);
	fclose(f);
}

extern const hkClass MyVertElementClass;
extern const hkClass MySimpleBufferClass;
extern const hkClass MyVertexDataStorageClass;
//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}

//Test>>>>>>>>>
/*LPD3DXMESH pMesh=NULL;
LPD3DXBUFFER pPShaderBuf=NULL;
LPDIRECT3DPIXELSHADER9 pPShader9=NULL;
LPD3DXCONSTANTTABLE pPSConstantTable=NULL;

LPDIRECT3DTEXTURE9 pTexDiff=NULL;
LPDIRECT3DTEXTURE9 pTexNorm=NULL;

LPD3DXBUFFER	pVShaderBuf=NULL;
LPDIRECT3DVERTEXSHADER9 pVShader9=NULL;
LPD3DXCONSTANTTABLE pVSConstantTable=NULL;


ID3DXEffect* g_pEffect = NULL;
*/
LPDIRECT3DTEXTURE9 pMatMap=NULL;

//LPD3DXCONSTANTTABLE pVSConstantTable=NULL;
BONE *pTestSkeleton;
long NumTestBones;
LPD3DXMATRIXSTACK pTestMatrixStack;
//<<<<<<<<

//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


namespace Test{
	hkResource* res;
	const char *ch;
	hkRootLevelContainer* container;
	hkxScene *scene;
	GActor dummy;
}
//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	g_pCurDevice=pd3dDevice;
	g_renderManager.Setup(pd3dDevice);
	using namespace Test;
	g_renderManager.SetCamera(scene->m_cameras[0]);
	g_renderManager.AddLight(scene->m_lights[0]);
	g_renderManager.SetAmbient(D3DXVECTOR3(0.3f,0.0f,0.0f));
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	if(FAILED(D3DXCreateTexture(pd3dDevice,4,1024,0,D3DUSAGE_DYNAMIC,D3DFMT_A32B32G32R32F,D3DPOOL_DEFAULT,&pMatMap))){
		exit(15);
	}
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	g_physicsWorld->stepDeltaTime(fElapsedTime);
//	pTestSkeleton->Rotation.w+=0.01;
	D3DXQUATERNION delta;
	D3DXQuaternionRotationAxis(&delta,&D3DXVECTOR3(0,0,1),0.005);
	pTestSkeleton[1].Rotation*=delta;
	pTestMatrixStack->LoadIdentity();
	BONE::MatStack=pTestMatrixStack;
	BONE::cur=pTestSkeleton;
	BONE::MaxBoneID=-1;
	pTestSkeleton->FillMatrices();
	{
		D3DLOCKED_RECT rect;
		pMatMap->LockRect(0,&rect,0,D3DLOCK_DISCARD);
		memcpy(rect.pBits,BONE::FinalMats,(BONE::MaxBoneID+1)*64);
		pMatMap->UnlockRect(0);
	}
	{
		using namespace Test;
		(dummy.*dummy.UpdateWorld)();
	}
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 100 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		pd3dDevice->SetRenderState(D3DRS_COLORVERTEX,FALSE);
		pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		for(long i=0;i<8;++i){
			pd3dDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
			pd3dDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
		}
		{
			using namespace Test;
			(dummy.*dummy.UpdateScene)();
		}
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	pMatMap->Release();
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	{
		D3DXCreateMatrixStack(0,&g_pMatrixStack);
		HRESULT h=g_pMatrixStack->LoadIdentity();
//		D3DXMATRIX *p=g_pMatrixStack->GetTop();
//		p=p;
	}
	{
		FILE *f=fopen("HavokError.log","w");
		fclose(f);
	}
	//
	// Initialize the base system including our memory system
	//

	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault();
	hkBaseSystem::init( memoryRouter, HavokErrorToLog );

	hkJobThreadPool* threadPool;
	int totalNumThreadsUsed;

	hkHardwareInfo hwInfo;
	hkGetHardwareInfo(hwInfo);
	totalNumThreadsUsed = hwInfo.m_numThreads;

	hkCpuJobThreadPoolCinfo threadPoolCinfo;
	threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

	threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
	threadPool = new hkCpuJobThreadPool( threadPoolCinfo );
	
	hkJobQueueCinfo info;
	info.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
	hkJobQueue* jobQueue = new hkJobQueue(info);

	hkMonitorStream::getInstance().resize(200000);

	{
		hkpWorldCinfo worldInfo;
		worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
		worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
		worldInfo.m_broadPhaseWorldAabb = hkAabb(hkVector4(-100,-100,-100,1),hkVector4(10000,10000,1000,1));
		worldInfo.m_collisionTolerance = 0.02f;
		worldInfo.m_iterativeLinearCastEarlyOutDistance = 0.002f;
		worldInfo.m_contactRestingVelocity = 0.5f;
		worldInfo.m_expectedMaxLinearVelocity = 1000.0f;
		worldInfo.m_useKdTree = true;
		worldInfo.setBroadPhaseWorldSize(10000.0f);
		worldInfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_HARD);
		g_physicsWorld = new hkpWorld(worldInfo);
		// Register all collision agents, even though only box - box will be used in this particular example.
		// It's important to register collision agents before adding any entities to the world.
		g_physicsWorld->markForWrite();
		hkpAgentRegisterUtil::registerAllAgents( g_physicsWorld->getCollisionDispatcher() );
		g_physicsWorld->registerWithJobQueue( jobQueue );
//		setupPhysics( physicsWorld );
	}

	{		
		
		#define HK_CUSTOM_CLASS(C) \
			extern const hkTypeInfo C##TypeInfo;					\
			extern const hkClass C##Class;							\
			hkBuiltinTypeRegistry::getInstance().addType( &(C##TypeInfo), &(C##Class) );	
		HK_CUSTOM_CLASS(MySimpleBuffer)
		HK_CUSTOM_CLASS(MyVertElement)
		HK_CUSTOM_CLASS(MyVertexDataStorage)
		HK_CUSTOM_CLASS(MyMaterialExtra)
	}
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	{
		using namespace Test;
		res = hkSerializeUtil::load("havoktest_Xport.hkx");
		ch=res->getContentsTypeName();
		container = res->getContents<hkRootLevelContainer>();
		scene=static_cast<hkxScene*>(container->findObjectByName("Scene Data"));
		dummy.m_rootNode=scene->m_rootNode;
		hkxNode *t=dummy.m_rootNode;
		hkxScene *s=scene;
	}
	{
		D3DXCreateMatrixStack(0,&pTestMatrixStack);
		SkeletonFromFile(&pTestSkeleton,&NumTestBones,"test.dxb");
	}
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( false, true, false);  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Game" );
	{
		FILE *f=fopen("config.txt","r");
		char cmd[128];
		while(fscanf(f,"%s",cmd)!=EOF){
			
		}
		fclose(f);
		DXUTCreateDevice( g_bWindowed, g_iScreenWidth, g_iScreenHeight);
//		MeshFromFile(&pMesh,"test.dxm");
	
	}
    // Start the render loop
    DXUTMainLoop();

     return DXUTGetExitCode();
}
