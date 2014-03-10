#include "DXUT.h"
#include "index.h"
#include "GActor.h"
#include "GRenderManager.h"
#include <D:\MyDemo\Common\SerializedObjects.h>
#include <Common/Base/KeyCode.h>
//#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

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
#include <Common/SceneData/Graph/hkxNode.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Common/SceneData/Mesh/hkxIndexBuffer.h>
#include "hkxMaterialX.h"
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>// Register Havok classes.
#include <Common/Base/Container/Array/hkArray.h>
#include <D:/MyDemo/Common/MyMaterialExtra.h>
#include <typeinfo.h>
#include "GRunTimeDataPack.h"


GActor::GActor():m_rootNode(NULL),UpdateWorld(&GActor::GeneralUpdate),UpdateScene(&GActor::GeneralDraw){
//	UpdateWorld=&GActor::GeneralUpdate;
}
namespace GActor_GeneralUpdate{
}
void GActor::GeneralUpdate(){
	using namespace GActor_GeneralUpdate;
	GeneralUpdate_ProcessNode(m_rootNode);
}
void GActor::GeneralUpdate_ProcessNode(hkxNode *node){
	using namespace GActor_GeneralUpdate;
}
namespace GActor_GeneralDraw{
}
void GActor::GeneralDraw(){
	using namespace GActor_GeneralDraw;
	GeneralDraw_ProcessNode(m_rootNode);
}
//extern LPD3DXCONSTANTTABLE pVSConstantTable;
//extern LPD3DXCONSTANTTABLE pPSConstantTable;
extern GRenderManager g_renderManager;
void GActor::GeneralDraw_ProcessNode(hkxNode *node){
	LPDIRECT3DDEVICE9 device= DXUTGetD3D9Device();
	using namespace GActor_GeneralDraw;
	g_pMatrixStack->Push();
	g_pMatrixStack->MultMatrix(reinterpret_cast<D3DXMATRIX*>(&node->m_keyFrames[0]));
	//device->SetTransform(D3DTS_WORLD,g_pMatrixStack->GetTop());
	for(long i=0;i<node->m_keyFrames.getSize();++i){
		GeneralDraw_ProcessNode(node->m_children[i]);
	}
	if(node->m_object.getClass()==&hkxMeshClass){
		g_renderManager.SetWorldTransform(g_pMatrixStack->GetTop());
		hkxMesh *pMesh=(hkxMesh *)(node->m_object.val());
		for(long iSec=0;iSec<pMesh->m_sections.getSize();++iSec){
			hkxMeshSection *pSec=pMesh->m_sections[iSec];
			MyVertexDataStorage *pStorage=(MyVertexDataStorage *)(pSec->m_userChannels[0].val());
			for(long iBuf=0;iBuf<pStorage->m_numVbs;++iBuf){
				if(pStorage->m_Vbs[iBuf]==NULL){
					device->CreateVertexBuffer(pStorage->m_buffer[iBuf].m_numData,0,0,D3DPOOL_MANAGED,pStorage->m_Vbs+iBuf,NULL);
					LPDIRECT3DVERTEXBUFFER9 vb=pStorage->m_Vbs[iBuf];
					void *p;
					vb->Lock(0,0,&p,0/*D3DLOCK_NOSYSLOCK*/);
					memcpy(p,pStorage->m_buffer[iBuf].m_data,pStorage->m_buffer[iBuf].m_numData);
					vb->Unlock();
				}
				device->SetStreamSource(iBuf,pStorage->m_Vbs[iBuf],0,pStorage->m_buffer[iBuf].m_numData/pStorage->m_vertCount);
			}
			if(pStorage->m_Ibs[0]==NULL){
				long stride;//=pSec->m_indexBuffers[0]->m_length<=65000?sizeof(WORD):sizeof(DWORD);
				D3DFORMAT Format;//=pSec->m_indexBuffers[0]->m_length<=65000?D3DFMT_INDEX16;D3DFMT_INDEX32;
				if(pSec->m_indexBuffers[0]->m_length<=65000){
					stride=sizeof(WORD);
					Format=D3DFMT_INDEX16;
				}else{
					stride=sizeof(DWORD);
					Format=D3DFMT_INDEX32;
				}
				long size=stride*pSec->m_indexBuffers[0]->m_length;
				device->CreateIndexBuffer(size,0,Format,D3DPOOL_MANAGED,pStorage->m_Ibs,NULL);
				LPDIRECT3DINDEXBUFFER9 ib=pStorage->m_Ibs[0];
				void *p;
				ib->Lock(0,0,&p,0/*D3DLOCK_NOSYSLOCK*/);
				if(Format==D3DFMT_INDEX16){
					memcpy(p,pSec->m_indexBuffers[0]->m_indices16.begin(),size);
				}else{
					memcpy(p,pSec->m_indexBuffers[0]->m_indices32.begin(),size);
				}
				ib->Unlock();
			}
			device->SetIndices(pStorage->m_Ibs[0]);
			if(pStorage->m_Dec==NULL){
				device->CreateVertexDeclaration((D3DVERTEXELEMENT9*)pStorage->m_decl,&pStorage->m_Dec);
			}
			device->SetVertexDeclaration(pStorage->m_Dec);
			{
				D3DXMATRIX * t=g_pMatrixStack->GetTop();
				t=t;
			}
			((hkxMaterialX*)pSec->m_material.val())->SetDeviceState();
//			LPD3DXEFFECT pEffect=((MyMaterialExtra*)pSec->m_material->m_extraData.m_object)->m_pRunTime->m_pEffect;
			if(typeid(*((MyMaterialExtra*)pSec->m_material->m_extraData.val())->m_pRunTime)==typeid(GRunTimeDataPackWithEffect)){
				UINT nPasses;				
				GRunTimeDataPackWithEffect *pPack=(GRunTimeDataPackWithEffect *)((MyMaterialExtra*)pSec->m_material->m_extraData.val())->m_pRunTime;
				if(pPack->m_technique){
					pPack->m_pEffect->SetTechnique(pPack->m_technique);
				}
				pPack->m_pEffect->Begin(&nPasses,D3DXFX_DONOTSAVESTATE);
				for(UINT i=0;i<nPasses;++i){
					pPack->m_pEffect->BeginPass(i);
					device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,pStorage->m_vertCount,0,pSec->m_indexBuffers[0]->m_length/3);
					pPack->m_pEffect->EndPass();
				}
				pPack->m_pEffect->End();
			}else{
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,pStorage->m_vertCount,0,pSec->m_indexBuffers[0]->m_length/3);
			}
		}
	}
	g_pMatrixStack->Pop();
}

