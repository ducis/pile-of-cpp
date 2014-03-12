#ifndef _GEOM_SHAPE_H_
#define _GEOM_SHAPE_H_

#include "vr.h"
#include "vrAnimationNode.h"

#define XX 0
#define YY 1
#define ZZ 2
#define WW 3

#define PI               3.14159265359
#define CONE_RESOLUTION 10
#define CYLINDER_RESOLUTION 10
#define RING_NUMBER     6
#define SECTION_NUMBER  10


typedef enum {
	VR_BOX = VR_ROUTE+1, VR_CONE=VR_ROUTE+2,VR_CYLINDER=VR_ROUTE+3,
	VR_SPHERE=VR_ROUTE+4,VR_EXTRUSION=VR_ROUTE+5,	VR_ELEVATION_GRID=VR_ROUTE+6,
	VR_INDEXED_LINE_SET=VR_ROUTE+7, VR_POINT_SET =VR_ROUTE+8, VR_PROTO_CALL=VR_ROUTE+9
} VRAdditionalNodeType;

class VRBox:public VRGeometry{
public:
	VRSFVec3f size;
	VRBox();
	void Copy(VRBox* src);
	void release();
	bool getBoundingBox(float [],float []);

};

class VRCone:public VRGeometry{
public:
	VRSFFloat bottomRadius;
	VRSFFloat height;
	VRSFBool side;
	VRSFBool bottom;
	VRCone();
	void Copy(VRCone* src);
	void release();
	bool getBoundingBox(float [],float []);
};

class VRSphere:public VRGeometry{
public:
	VRSFFloat radius;
	VRSphere();
	void Copy(VRSphere* src);
	void release();
	bool getBoundingBox(float [],float []);
};

class VRExtrusion:public VRGeometry{
public:
	VRMFVec3f spine;
	VRMFVec2f crossSection;
	VRMFVec2f scale;
	VRMFRotation orientation;
	VRSFBool beginCap;
	VRSFBool endCap;
	VRSFBool ccw;
	VRSFBool solid;
	VRSFBool convex;
	VRSFFloat creaseAngle;
	VRExtrusion();
	void Copy(VRExtrusion* src);
	void release();
	bool getBoundingBox(float [],float []);
};

class VRCylinder:public VRGeometry{
public:
	VRSFFloat radius;
	VRSFFloat height;
	VRSFBool side;
	VRSFBool top;
	VRSFBool bottom;
	VRCylinder();
	void Copy(VRCylinder* src);
	void release();
	bool getBoundingBox(float [],float []);
};

class VRElevationGrid:public VRGeometry{
public:
	VRSFInt32 xDimension;
	VRSFFloat xSpacing;
	VRSFInt32 zDimension;
	VRSFFloat zSpacing;
	VRMFFloat height;
	VRSFNode color;
	VRSFBool colorPerVertex;
	VRSFNode normal;
	VRSFBool normalPerVertex;
	VRSFNode texCoord;
	VRSFBool ccw;
	VRSFBool solid;
	VRSFFloat creaseAngle;
	VRElevationGrid();
	void Copy(VRElevationGrid* src);
	void release();
	bool getBoundingBox(float [],float []);
};

class VRIndexedLineSet:public VRGeometry{
public:
	VRIndexedLineSet();
	void Copy(VRIndexedLineSet* src);
	void release();
	bool getBoundingBox(float [],float []);
};

class VRPointSet:public VRGeometry{
public:
	VRPointSet();
	void Copy(VRPointSet* src);
	void release();
	bool getBoundingBox(float [],float []);
};

VRIndexedFaceSet* box2ifs(VRBox* box);

VRIndexedFaceSet* cone2ifs(VRCone* cone,int resolution=CONE_RESOLUTION);

VRIndexedFaceSet* cylinder2ifs(VRCylinder* cylinder,int resolution=CYLINDER_RESOLUTION);

VRIndexedFaceSet* sphere2ifs(VRSphere* sphere,int num_rings=RING_NUMBER,int num_sections=SECTION_NUMBER);

VRIndexedFaceSet* elevationGrid2ifs(VRElevationGrid * grid);

VRIndexedFaceSet* extrusion2ifs(VRExtrusion * extrusion);

#endif