#pragma once
#include <OGRE.h>
#include "OCCTMeshUtils.hpp"
#include <boost/functional/value_factory.hpp>
#include <BRepMesh.hxx>

namespace OCCTMesh2OGRE{
	inline void FillBlankOgreMeshUsingMeshedOCCTShape(Ogre::MeshPtr pMesh,const TopoDS_Shape &s)
	{	
		using namespace OpenCASCADEUtils;
		int nV = GetMeshedShapeVertCount(s);
		int nF = GetMeshedShapeTriangleCount(s);
		assert(nV&&nF);

		Ogre::SubMesh *pM = pMesh->createSubMesh();
		assert(true == pM->useSharedVertices);
		pM->useSharedVertices = false;
		assert(!pM->vertexData);
		pM->vertexData = new Ogre::VertexData;
		pM->vertexData->vertexStart = 0;
		pM->vertexData->vertexCount = nV;

		assert(pM->vertexData->vertexDeclaration);
		assert(pM->vertexData->vertexBufferBinding);
		assert(pM->vertexData->mDeleteDclBinding);
		{
			Ogre::VertexDeclaration *pVD = pM->vertexData->vertexDeclaration;
			assert(0 == pVD->getElementCount());
			size_t s = pVD->addElement(0,0,Ogre::VET_FLOAT3,Ogre::VES_POSITION).getSize();
			assert(3*sizeof(float)==s);
			s = pVD->addElement(1,0,Ogre::VET_FLOAT3,Ogre::VES_NORMAL).getSize();
			assert(3*sizeof(float)==s);
		}

		Ogre::HardwareVertexBufferSharedPtr vbPos = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(3*sizeof(float),nV,Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		Ogre::HardwareVertexBufferSharedPtr vbNrm = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(3*sizeof(float),nV,Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		{
			Ogre::VertexBufferBinding *pVBB = pM->vertexData->vertexBufferBinding;
			assert(0 == pVBB->getBufferCount());
			pVBB->setBinding(0,vbPos);
			pVBB->setBinding(1,vbNrm);
		}

		pM->indexData->indexStart = 0;
		pM->indexData->indexCount = 3*nF;
		assert(pM->indexData->indexBuffer.isNull());
		Ogre::HardwareIndexBufferSharedPtr ib = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT,nF*3,Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		pM->indexData->indexBuffer = ib;


		pMesh->_setBoundingSphereRadius(sqrt(GetMeshedShapeMaxVertexDistanceSquared(s)));
		pMesh->_setBounds(boost::fusion::invoke(boost::value_factory<Ogre::AxisAlignedBox>(),GetMeshedShapeAABB(s)));

		CopyComponentsOfMeshedShapeVertexPositions(static_cast<float*>(vbPos->lock(Ogre::HardwareBuffer::HBL_NORMAL)),s);
		vbPos->unlock();

		CopyComponentsOfMeshedShapeVertexNormals(static_cast<float*>(vbNrm->lock(Ogre::HardwareBuffer::HBL_NORMAL)),s);
		vbNrm->unlock();

		CopyMeshedShapeIndices(static_cast<Ogre::int32*>(ib->lock(Ogre::HardwareBuffer::HBL_NORMAL)),s);
		ib->unlock();
	}
	inline void FillBlankOgreMeshUsingOCCTShape(Ogre::MeshPtr pMesh,const TopoDS_Shape &s,Standard_Real meshingDeflection){
		TopoDS_Shape ss = s;
		BRepMesh::Mesh(ss,meshingDeflection);
		FillBlankOgreMeshUsingMeshedOCCTShape(pMesh,ss);
	}
}