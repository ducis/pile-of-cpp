
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "MyMeshConverter.h"
#include <D:/MyDemo/Common/SerializedObjects.h>
#include <D:/MyDemo/Common/MyMaterialExtra.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Common/SceneData/Mesh/hkxIndexBuffer.h>
#include <Common/SceneData/Mesh/hkxVertexBuffer.h>
#include <Common/SceneData/Mesh/hkxVertexDescription.h>
#include <Common/SceneData/Material/hkxMaterial.h>
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Graph/hkxNode.h>
#include <d3d9types.h>
#include <d3d9.h>
extern const hkClass MyVertexDataStorageClass;
extern const hkClass MyMaterialExtraClass;
MyMeshConverter::MyMeshConverter(const hctFilterManagerInterface* owner):	hctFilterInterface (owner)
{
}

MyMeshConverter::~MyMeshConverter(){
}
void ProcessNode(hkxNode *pNode,hkxScene *pScene){
//	for(long i=0;i<pNode->m_num
	for(long i=0;i<pNode->m_children.getSize();++i){
		ProcessNode(pNode->m_children[i],pScene);	
	}
}

const D3DVERTEXELEMENT9 null_terminator={0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0};
//hkClass MyVertexDataStorageClass("MyVertexDataStorageClass",HK_NULL,sizeof(MyVertexDataStorage),NULL,0,NULL,0,;
void MyMeshConverter::process( class hkRootLevelContainer& data ){
	HK_REPORT("MyMeshConverter");
	hkxScene *scene=reinterpret_cast<hkxScene*>(data.findObjectByType(hkxSceneClass.getName()));
	for(long i=0;i<scene->m_materials.getSize();++i){
		//scene->m_materials[i]->m_extraData.getClass()=&MyMaterialExtraClass;
		//scene->m_materials[i]->m_extraData.val()=new MyMaterialExtra;
		scene->m_materials[i]->m_extraData.set(new MyMaterialExtra,&MyMaterialExtraClass);
	}
	for(long i=0;i<scene->m_meshes.getSize();++i){
		for(long j=0;j<scene->m_meshes[i]->m_sections.getSize();++j){
			hkxMeshSection *section=scene->m_meshes[i]->m_sections[j];

			hkxVertexBuffer *hVB=section->m_vertexBuffer;
		//	hVB->getVertexDesc();

			MyVertexDataStorage *storage=new MyVertexDataStorage;
			memset(storage,0,sizeof(MyVertexDataStorage));
			scene->m_meshes[i]->m_sections[j]->m_userChannels.setSize(1) ;
			//scene->m_meshes[i]->m_sections[j]->m_userChannels[0].m_object=storage;
			//scene->m_meshes[i]->m_sections[j]->m_userChannels[0].m_class=&MyVertexDataStorageClass;
			scene->m_meshes[i]->m_sections[j]->m_userChannels[0].set(storage,&MyVertexDataStorageClass);
			BYTE *ptrBuf[256];
			//long bufStride[256];
			D3DVERTEXELEMENT9 declBuf[256];
			long hVBoffset[256];
			long toCopy[256];
			long toMove[256];
			long numBuffer=0;
			long numDecl=0;
			memset(declBuf,0,sizeof(declBuf[0]));
			storage->m_vertCount=hVB->getNumVertices();
			const hkxVertexDescription *hVBdesc=&hVB->getVertexDesc();
			
			for(const hkxVertexDescription::ElementDecl *element=hVBdesc->m_decls.begin();element!=hVBdesc->m_decls.end();++element){
				long cur=numDecl;
				hVBoffset[cur]=element->m_byteOffset;
				if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT){
					declBuf[cur].Type=D3DDECLTYPE_FLOAT1;
					toMove[cur]=sizeof(float);
				}else if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT2){
					declBuf[cur].Type=D3DDECLTYPE_FLOAT2;
					toMove[cur]=2*sizeof(float);
				}else if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT3){
					declBuf[cur].Type=D3DDECLTYPE_FLOAT3;
					toMove[cur]=3*sizeof(float);
				}else if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT4){
					declBuf[cur].Type=D3DDECLTYPE_FLOAT4;
					toMove[cur]=4*sizeof(float);
				}
				if(element->m_usage==hkxVertexDescription::HKX_DU_POSITION){
					declBuf[cur].Usage=D3DDECLUSAGE_POSITION;
					if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT4){
						declBuf[cur].Type=D3DDECLTYPE_FLOAT3;
					}
				}else if(element->m_usage==hkxVertexDescription::HKX_DU_NORMAL){
					declBuf[cur].Usage=D3DDECLUSAGE_NORMAL;
					if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT4){
						declBuf[cur].Type=D3DDECLTYPE_FLOAT3;
					}
				}else if(element->m_usage==hkxVertexDescription::HKX_DU_TANGENT){
					declBuf[cur].Usage=D3DDECLUSAGE_TANGENT;
					if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT4){
						declBuf[cur].Type=D3DDECLTYPE_FLOAT3;
					}
				}else if(element->m_usage==hkxVertexDescription::HKX_DU_BINORMAL){
					declBuf[cur].Usage=D3DDECLUSAGE_BINORMAL;
					if(element->m_type==hkxVertexDescription::HKX_DT_FLOAT4){
						declBuf[cur].Type=D3DDECLTYPE_FLOAT3;
					}
				}else if(element->m_usage==hkxVertexDescription::HKX_DU_TEXCOORD){
					declBuf[cur].Usage=D3DDECLUSAGE_TEXCOORD;
					toMove[cur]=4*sizeof(float);
				}
				short sizeTbl[]={sizeof(float),2*sizeof(float),3*sizeof(float),4*sizeof(float),sizeof(D3DCOLOR),4,2*sizeof(short),4*sizeof(short)};
				toCopy[numDecl]=sizeTbl[declBuf[numDecl].Type];

				declBuf[numDecl+1]=declBuf[numDecl];
				++numDecl;

				declBuf[numDecl].Offset+=sizeTbl[declBuf[numDecl].Type];
				if(declBuf[numDecl].Offset>bufStride[declBuf[numDecl].Stream])
					bufStride[declBuf[numDecl].Stream]=declBuf[numDecl].Offset;
			}

			ptrBuf[numBuffer]=static_cast<BYTE*>(malloc(storage->m_vertCount*bufStride[numBuffer]));
			++numBuffer;
			
			storage->m_buffers.setSize(numBuffer);
			for(long i=0;i<numBuffer;++i){
				long t=storage->m_vertCount*bufStride[i];
				storage->m_buffers[i]=new hkArray<BYTE>(ptrBuf[i],t,t);
			}
			char *hVBdata=hVB->getVertexDataPtr(hVB->getVertexDesc());
			for(long i=0;i<storage->m_vertCount;++i){
				for(long j=0;j<numDecl;++j){
					long s=declBuf[j].Stream;
					memcpy(ptrBuf[s],hVBdata,toCopy[j]);
					ptrBuf[s]+=toCopy[j];
					hVBdata+=toMove[j];
				}
			}
			declBuf[numDecl++]=null_terminator;
			assert(sizeof(MyVertElement)==sizeof(D3DVERTEXELEMENT9);
			//storage->m_decl	=reinterpret_cast<MyVertElement *>(new D3DVERTEXELEMENT9[storage->m_numDecl]);
			storage->m_decl.setSize(numDecl);
			memcpy(storage->m_decl.begin(),declBuf,sizeof(D3DVERTEXELEMENT9)*numDecl);
			storage->m_numIbs=section->m_numIndexBuffers;
			storage->m_numVbs=numBuffer;
			storage->m_Ibs=new LPDIRECT3DINDEXBUFFER9[storage->m_numIbs];
			memset(storage->m_Ibs,0,sizeof(LPDIRECT3DINDEXBUFFER9)*storage->m_numIbs);
			storage->m_Vbs=new LPDIRECT3DVERTEXBUFFER9[storage->m_numVbs];
			memset(storage->m_Vbs,0,sizeof(LPDIRECT3DVERTEXBUFFER9)*storage->m_numVbs);
			//D3DVERTEXELEMENT9
		}
	}
	ProcessNode(scene->m_rootNode,scene);
}

MyMeshConverterDescription g_MyMeshConverterDesc;
