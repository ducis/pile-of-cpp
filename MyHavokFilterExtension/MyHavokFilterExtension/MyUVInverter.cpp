
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "MyUVInverter.h"
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

MyUVInverter::MyUVInverter(const hctFilterManagerInterface* owner):	hctFilterInterface (owner)
{
}

MyUVInverter::~MyUVInverter(){
}

void MyUVInverter::process(hkRootLevelContainer &data){
	HK_REPORT("MyUVInverter");
	hkxScene *scene=reinterpret_cast<hkxScene*>(data.findObjectByType(hkxSceneClass.getName()));
	
	for(long i=0;i<scene->m_numMeshes;++i){
		for(long j=0;j<scene->m_meshes[i]->m_numSections;++j){
			hkxMeshSection *section=scene->m_meshes[i]->m_sections[j];
			hkxVertexBuffer *hVB=section->m_vertexBuffer;
			
			hkxVertexDescription *hVBdesc=hVB->getVertexDesc();
			const hkClass *klass=hVB->getVertexDataClass();
			long n=hVB->getNumVertices();
			for(hkxVertexDescription::ElementDecl *element=hVBdesc->m_decls.begin();element!=hVBdesc->m_decls.end();++element){
				if(element->m_usage==hkxVertexDescription::HKX_DU_TEXCOORD){
					char *p=hVB->getVertexData();
					for(long i=0;i<n;++i){
						float *t=(float*)(p+element->m_byteOffset);
						{
							t[1]=-t[1];
						}
						p+=hVBdesc->m_stride;
					}
				}
			}
		}
	}
}
MyUVInverterDescription g_MyUVInverterDesc;
