#ifndef _VRINDEXEDFACESET_H_
#define _VRINDEXEDFACESET_H_

#include "vrNode.h"

struct VRVoxel;

// VRIndexedFaceSet::validity_tests() : error treatment policy
typedef enum {	VR_ER_WARN_ONE,	// warn (prinouts or message-box) about the errors
				VR_ER_WARN_ALL,	// warn (prinouts or message-box) about the errors
				VR_ER_FIX,		// fix the error silently
				VR_ER_WARN_ALL_FIX,	// fix the error and warn
} VRErrorHandle;

	// VRIndexedFaceSet::validity_tests() : returned error details
typedef enum {	VR_IFS_OK,			// passed tests
				VR_IFS_MINOR,		// minor or local error (s.a. huge-length normal)
				VR_IFS_LENGTH,		// length of vector inconsistent
				VR_IFS_RANGE,		// out-of-range for index or coordinate
				VR_IFS_EMPTY,		// model lacks vertices or facets
				VR_IFS_FATAL		// missing or maleficient data, etc.
} VRIFSError;


class VRIndexedFaceSet: public VRGeometry
{
public:
	VRIndexedFaceSet(void);
	~VRIndexedFaceSet(){};

	virtual void release(void);
	VRDWORD weld(VRSFFloat tolerance);
	VRResult triangulate();
	bool getBoundingBox(VRSFVec3f bottom, VRSFVec3f top);
	void applyTextureTransform(VRTextureTransform*);


	// Validity tests include nodes' length and range tests, indices correlations,
	// multiple-similar-vertex in a facet and degenerate facet.
	VRIFSError  validity_tests( char *err_msg);

	void Copy(VRIndexedFaceSet* src);

	VRCoordinate*			coord;
    VRMFInt32				coordIndex;
	VRTextureCoordinate*	texCoord;
	VRMFInt32				texCoordIndex;
	VRColor*				color;
	VRMFInt32				colorIndex;
    VRSFBool				colorPerVertex;
	VRNormal*				normal;
	VRMFInt32				normalIndex;
    VRSFBool				normalPerVertex;
	VRSFBool				ccw;
	VRSFBool				solid;
	VRSFBool				convex;
	VRSFFloat				creaseAngle;
private:
	VRErrorHandle err_treat;	// default: ER_WARN - warn (prinouts or message-box) about errors

	VRIFSError  error_treat(char *err_msg, VRIFSError err, VRErrorHandle treat, char *msg);
	bool identicalProperties(VRDWORD ind1, VRDWORD ind2, 
					   bool ensureVerColor = true, bool ensurePolColor = true,
					   bool ensureTexCoord = true, bool ensureNormal = true) ;
	void voxelization(VRVoxel voxels[], VRSFFloat &eps, VRSFFloat tolerance);

};

#endif
