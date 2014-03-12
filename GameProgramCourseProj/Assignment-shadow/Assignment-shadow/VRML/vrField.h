//***************************************************************//
//*																*//
//*  Copyright (C) 1998 Virtue 3D, Inc.   All Rights Reserved.	*//
//*																*//
//*  File:	   vrField.h										*//
//*  Content:  basic types, arrays of types, node types			*//
//*  Date:     October 26, 1998									*//
//*																*//
//***************************************************************//
//
// HISTORY :
//	Ophir 8/10/98 :
//		Added a  Copy()  constructor to all classes.
//


#ifndef _VR_FIELD_
#define _VR_FIELD_

#include <stdio.h>

#define  CopyFloat(d,s,n)		memcpy( (void*)d, (void*)s, (n)*(sizeof(float)))
#define  WNULL					((VRDWORD *)0)

typedef enum { VR_OK, VR_ERR } VRResult;
typedef char*     VRSFString;
typedef bool      VRSFBool;
typedef long      VRSFInt32;
typedef long	  VRTriangle[3];
typedef float     VRSFFloat;
typedef float     VRSFVec2f[2];
typedef float     VRSFVec3f[3];
typedef float     VRSFVec4f[4];
typedef float     VRSFColor[3];
typedef float     VRSFRotation[4];
typedef double    VRSFTime;
typedef float     VRSFMat44f[4][4];		// for  Transform Matrix  operations
typedef unsigned long VRDWORD;
typedef unsigned short ushort;
typedef unsigned char  uchar;


class VRNode;
typedef VRNode*  VRSFNode;


class VRMFInt32
{
public:
	VRMFInt32(void);
	void Copy(VRMFInt32* src);

	VRDWORD    numValues;
	VRSFInt32* values;
};

class VRMFDWORD
{
public:
	VRMFDWORD(void);
	void Copy(VRMFDWORD* src);

	VRDWORD    numValues;
	VRDWORD   *values;
};

class VRMFFloat
{
public:
	VRMFFloat(void);
	void Copy(VRMFFloat* src);

	VRDWORD    numValues;
	VRSFFloat* values;
};

class VRMFVec2f
{
public:
	VRMFVec2f(void);
	void Copy(VRMFVec2f* src);

	VRDWORD    numValues;
	VRSFVec2f* values;
};

class VRMFVec3f
{
public:
	VRMFVec3f(void);
	void Copy(VRMFVec3f* src);

	VRDWORD    numValues;
	VRSFVec3f* values;
};

class VRMFString
{
public:
	VRMFString(void);
	void Copy(VRMFString* src);

	VRDWORD     numValues;
	VRSFString* values;
};

class VRMFColor
{
public:
	VRMFColor(void);
	void Copy(VRMFColor* src);

	VRDWORD    numValues;
	VRSFColor* values;
};

class VRMFNode
{
public:
	VRMFNode(void);
	void Copy(VRMFNode* src);

	VRDWORD   numValues;
	VRSFNode* values;
};

class VRMFRotation
{
public:
	VRMFRotation(void);
	void Copy(VRMFRotation *src);

	VRDWORD       numValues;
	VRSFRotation* values;
};


class VRM2FVec3f
{
public:
	VRM2FVec3f(void);
	void Copy(VRM2FVec3f* src);

	VRDWORD    numValues[2];
	VRSFVec3f* values;
};


class VRMFTime
{
public:
	VRMFTime(void);
	void Copy(VRMFTime* src);

	VRDWORD   numValues;
	VRSFTime* values;
};
typedef enum {
	VR_NODE, VR_GROUP, VR_TRANSFORM, VR_LOD, VR_SHAPE, VR_APPEARANCE,
	VR_MATERIAL, VR_TEXTURE, VR_IMAGE_TEXTURE, VR_TEXTURE_TRANSFORM,
	VR_GEOMETRY, VR_INDEXED_FACE_SET, VR_COORDINATE, VR_TEXTURE_COORDINATE,
	VR_COLOR, VR_NORMAL, VR_VIEWPOINT,
	// Animation Nodes
	VR_INTERPOLATOR, VR_COORDINATE_INTERPOLATOR, VR_ORIENTATION_INTERPOLATOR,
	VR_POSITION_INTERPOLATOR, 	VR_SENSOR, VR_TIME_SENSOR, VR_ROUTE
} VRNodeType;

#endif	// _VR_FIELD_

