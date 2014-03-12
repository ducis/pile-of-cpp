//***************************************************************//
//*																*//
//*  Copyright (C) 1998 Virtue 3D, Inc.   All Rights Reserved.	*//
//*																*//
//*  File:	   vrNode.cpp										*//
//*  Content:  VR nodes source file								*//
//*  Date:     October 26, 1998									*//
//*																*//
//***************************************************************//
//
// HISTORY :
//	Ophir 24-29/9/98 :
//		Created  VRIndexedFaceSet::validity_tests()
//		   and	 VRIndexedFaceSet::error_treat()
//		   which tests model's basic correctness, and warn/fix minor problems.
//		Created  VRNode::getList(type)  and  VRNode::getShapes()
//		   which traverse the node-tree and return array of nodes of given type.
//	Ophir 6-8/10/98 :
//		Created  VRNode::TransformShapes()
//			which recursively traverse the tree and append shapes to the tail.
//			Multiply the Transformations on the path, and transform the shapes
//			by the result matrix, giving the new shapes.
//			Used by the (offline) occlusion algorithm.
//	Ophir 7-8/10/98 :
//		Added VRMatrix class + expanded functionality to "vrNode.h", "matrix.cpp" .
//		Added a  Copy()  constructor to major VR classes.


#ifndef  _VR_NODE_
#define  _VR_NODE_

#include <math.h>
#ifndef M_PI
#define M_PI  3.141592653589793
#endif

#include "VRField.h"


class VRNode;
class VRGroup;
class VRTransform;
class VRLOD;
class VRShape;
class VRAppearance;
class VRMaterial;
class VRTexture;
class VRImageTexture;
class VRTextureTransform;
class VRGeometry;
class VRIndexedFaceSet;
class VRCoordinate;
class VRTextureCoordinate;
class VRColor;
class VRNormal;
class VRMatrix;
class VRViewpoint;

#define PROTOS_SUPPORTED

#ifdef PROTOS_SUPPORTED
class UpdateList;
#endif

class VRNode
{
public:
	VRNode(void);
	virtual ~VRNode(void);

	virtual void release(void);
	void Copy(VRNode* src);

	// recursively traverse the tree and append shapes to the tail.
	// return total number of shapes found (length of chain)
	int getShapes( VRMFNode &head, VRDWORD &vindex);

	// get a shapes-list, and return node_list of type srch_type
	VRResult getNodesOfType( VRNodeType type, VRMFNode &nodes);

	// Recursively traverse the tree and append shapes to the tail.
	// Multiply the Transformations on the path, and transform the shapes
	// by the result matrix, giving the new shapes.
	// if(MakeCopy) then make a copy of the list shapes;  otherwise - work on current ones.
	VRResult transformShapes( VRShape ***shapes, VRDWORD &numShapes, VRSFBool makeCopy);

	// make one shape from the model
	VRResult uniteGeometry(VRShape **shape);

	// static funcion
	static VRResult releaseAuxiliaryMemory ();

	// data
	VRNode*    parent;
	VRSFString name;
	VRDWORD    referenceCount;
	VRNodeType type;

private:
	// see  above  transformShapes()
	int getTransformShapes( VRMFNode &head, VRDWORD *vindex, VRSFMat44f &tmat, VRSFBool makeCopy);
#ifdef PROTOS_SUPPORTED
public:
	UpdateList* update_records;
#endif
};

class VRGroup: public VRNode
{
public:
	VRGroup(void);
	~VRGroup(void){};
	virtual void release(void);
	void Copy(VRGroup* src);

	VRMFNode  children;
};

class VRTransform: public VRGroup
{
public:
	VRTransform(void);
	~VRTransform(){};
	void Copy(VRTransform* src);
	VRResult  reset();

	VRSFVec3f    translation;
	VRSFRotation rotation;
	VRSFVec3f    scale;
	VRSFRotation scaleOrientation;
	VRSFVec3f    center;
};

class VRLOD: public VRGroup
{
public:
	VRLOD(void);
	~VRLOD(){};
	virtual void release(void);
	void Copy(VRLOD* src);

	VRSFVec3f center;
	VRMFFloat range;
};

class VRShape: public VRNode
{
public:
	VRShape(void);
	~VRShape(){};
	virtual void release(void);
	VRResult decompose(VRShape** shape1, VRShape** shape2);
	
	void Copy(VRShape * src);
	VRShape * transformShape( VRSFMat44f mat, VRSFBool makeCopy);	// create (and return) a transformed shape
	void applyTextureTransform();

	VRAppearance* appearance;
	VRGeometry*   geometry;
};

class VRAppearance: public VRNode
{
public:
	VRAppearance(void);
	~VRAppearance(){};
	virtual void release(void);
	void Copy(VRAppearance* src);

	VRMaterial         *material;
	VRTexture          *texture;
	VRTextureTransform *textureTransform;
};

class VRMaterial: public VRNode
{
public:
	VRMaterial(void);
	~VRMaterial(){};
	void Copy(VRMaterial* src);

	VRSFColor diffuseColor;
	VRSFFloat ambientIntensity;
	VRSFColor specularColor;
	VRSFColor emissiveColor;
	VRSFFloat shininess;
	VRSFFloat transparency;
};

class VRTexture: public VRNode
{
public:
	VRTexture(void);
	~VRTexture(){};

	void Copy(VRTexture* src);

	VRSFBool repeatS;
	VRSFBool repeatT;
};

class VRImageTexture: public VRTexture
{
public:
	VRImageTexture(void);
	~VRImageTexture(){};
	void Copy(VRImageTexture* src);
	virtual void release(void);

	VRMFString url;
};

class VRTextureTransform: public VRNode
{
public:
	VRTextureTransform(void);
	~VRTextureTransform(){};
	void Copy(VRTextureTransform* src);
	void apply(VRSFVec2f);

	VRSFVec2f translation;
	VRSFFloat rotation;
	VRSFVec2f scale;
	VRSFVec2f center;
};

class VRGeometry: public VRNode
{
public:
	VRGeometry(void);
	~VRGeometry(){};

	virtual bool getBoundingBox(VRSFVec3f bottom, VRSFVec3f top) = 0;
	virtual void applyTextureTransform(VRTextureTransform*) {};
};


class VRCoordinate: public VRNode
{
public:
	VRCoordinate(void);
	~VRCoordinate(){};
	virtual void release(void);
	void Copy(VRCoordinate * src);

	VRMFVec3f point;
};

class VRTextureCoordinate: public VRNode
{
public:
	VRTextureCoordinate(void);
	~VRTextureCoordinate(){};
	virtual void release(void);
	void Copy(VRTextureCoordinate * src);

	VRMFVec2f point;
};

class VRColor: public VRNode
{
public:
	VRColor(void);
	~VRColor(){};
	virtual void release(void);
	void Copy(VRColor * src);

	VRMFColor color;
};

class VRNormal: public VRNode
{
public:
	VRNormal(void);
	~VRNormal(){};
	virtual void release(void);
	void Copy(VRNormal * src);

	VRMFVec3f vector;
};

class VRViewpoint: public VRNode
{
public:
	VRViewpoint(void);
	virtual void release();
	void Copy(VRViewpoint * src);

	VRSFVec3f    position;
	VRSFRotation orientation;
	VRSFFloat    fieldOfView;
	VRSFString   description;
};

#endif	// _VR_NODE_
