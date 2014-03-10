#ifndef INDEX_H
#define INDEX_H
// Math and base include
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>

// Dynamics includes
#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>			


#define MAX_DIRECTIONAL_LIGHT 8
extern LPD3DXMATRIXSTACK g_pMatrixStack;
struct BONE{
	long id;
	D3DXVECTOR3 Position;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Scale;
	long nC;
	D3DXMATRIX Inversion;
	void FillMatrices();
	void MakeReady(BONE *parent);
	static BONE* cur;
	static LPD3DXMATRIXSTACK MatStack;
	static LPD3DXCONSTANTTABLE ShaderConstantTable;
	static float FinalMats[1024][16];
	static long MaxBoneID;
};

struct ANIMATOR_KEYS{
	void *pKeys,*pOut;
	DWORD *pTimes;
	void (*pInterpolater)(void *,void *,void *,float);
	WORD flag,stride,nKeys,cur;
	DWORD length;
	void Update(DWORD t);
	void QuickSet(DWORD t);
};

void MeshFromFile(LPD3DXMESH *ppMesh,char *path);
void MeshFromFile(LPD3DXMESH *ppMesh,FILE *file);
void SkeletonFromFile(BONE **ppBoneArray,long *pNumBones,char *path);
#endif