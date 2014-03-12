#ifndef _VRML_LOADER_H
#define _VRML_LOADER_H

#include "Globals.h"
#include "VRML/vr.h"
#include "VRML/wrl.h"
#include <vector>


#ifndef _MESH
#define _MESH

struct BoundingBox {
	vector3 bll, blr, bul, bur; //back lower-left, back lower-right, back upper-left, back upper-right
	vector3 fll, flr, ful, fur; //front lower-left, front lower-right, front upper-left, front upper-right
};

struct Mesh {
	vector3 *vertices;
	vector3 *faces;
	vector3 *normals;

	unsigned int numVerts, numFaces;

	BoundingBox bb;

	VRIndexedFaceSet *ifset;

	Mesh() { vertices = 0; faces = 0; normals = 0; ifset = 0; numVerts = 0; numFaces = 0; }
};

#endif //_MESH

void computeNormals( Mesh &m );

void createArrays( VRIndexedFaceSet *ifset, Mesh &m );

std::vector < Mesh > loadVRML( char *filename );

#endif //_VRML_LOADER_H
