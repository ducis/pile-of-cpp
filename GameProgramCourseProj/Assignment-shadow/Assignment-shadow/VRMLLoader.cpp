#ifndef _VRML_LOADER_CPP
#define _VRML_LOADER_CPP

#include "Globals.h"
#include "VRMLLoader.h"
#include <memory.h>
#include <stdio.h>
#include <fstream>

void
computeNormals( Mesh &m ) {
	m.normals = new vector3[m.numVerts];
	// TODO: calculate the normals per vertex
	int *count=new int[m.numVerts];//counting uses extra memory by is very fast

	int i=0;

	for(i=0;i<m.numVerts;i++)
		count[i]=0;

	vector3 normal;

	for(i=0;i<m.numFaces;i++)//calculate running average by iterating faces
	{
		normal=CrossProduct( m.vertices[(int)m.faces[i].y] - m.vertices[(int) m.faces[i].x],
							  m.vertices[(int)m.faces[i].z] - m.vertices[(int) m.faces[i].x]).normalize();
		
		m.normals[((int)m.faces[i].x)]=(((m.normals[((int)m.faces[i].x)] * (float)(count[((int)m.faces[i].x)])) + normal)/(count[((int)m.faces[i].x)]+1)).normalize();
		m.normals[((int)m.faces[i].y)]=(((m.normals[((int)m.faces[i].y)] * (float)(count[((int)m.faces[i].y)])) + normal)/(count[((int)m.faces[i].y)]+1)).normalize();
		m.normals[((int)m.faces[i].z)]=(((m.normals[((int)m.faces[i].z)] * (float)(count[((int)m.faces[i].z)])) + normal)/(count[((int)m.faces[i].z)]+1)).normalize();
		count[((int)m.faces[i].x)]++;
		count[((int)m.faces[i].y)]++;
		count[((int)m.faces[i].z)]++;
	}

}

void 
createArrays( VRIndexedFaceSet *ifset, Mesh &m ) {
	m.ifset = ifset;
	m.numVerts = ifset->coord->point.numValues;
	m.vertices = new vector3[m.numVerts];
	for (int i = 0; i < m.numVerts; i++) {
		m.vertices[i].set(ifset->coord->point.values[i]);
	}

	m.numFaces = (int) (ifset->coordIndex.numValues * 0.25); // There is one face for every four of these values
	m.faces = new vector3[m.numFaces]; 
	for (int j = 0; j < ifset->coordIndex.numValues; j += 4) {
		m.faces[j / 4].set( ifset->coordIndex.values[j], ifset->coordIndex.values[j + 1], ifset->coordIndex.values[j + 2] );
	}
}

/**
 * To use the return value of loadVRML to render the geometry, you might do something like the following:
void renderGeometry() {
	glDisable( GL_LIGHTING );
	glColor3f( 0.2, 0.5, 0.5 );
	glBegin( GL_TRIANGLES );
		for ( int i = 0; i < meshes.size(); i++ ) {
			for ( int j = 0; j < meshes[i].numFaces; j++ ) {
				glVertex3fv( meshes[i].vertices[(int) meshes[i].faces[j].x].get() );
				glVertex3fv( meshes[i].vertices[(int) meshes[i].faces[j].y].get() );
				glVertex3fv( meshes[i].vertices[(int) meshes[i].faces[j].z].get() );
			}
		}
	glEnd();
}
*/

std::vector < Mesh >
loadVRML( char *filename ) {
	std::vector < Mesh > meshes;

	VRNode *root = Vrml2VR( filename );
	if (!root) {
		cout << "loadVRML: could not load file " << filename << endl;
		return meshes;
	}
	VRMFNode nodes;
	root->getNodesOfType( VR_INDEXED_FACE_SET, nodes );

	VRNode **sets = nodes.values;
	VRIndexedFaceSet **faceSets = (VRIndexedFaceSet**) sets;
	for (int i = 0; i < nodes.numValues; i++) {
		if (faceSets[i]->triangulate() != VR_OK) {
			cerr << "Unable to triangulate indexed face set " << i << " in " << filename << endl;
		}

		Mesh mesh;
		createArrays( faceSets[i], mesh );
		computeNormals( mesh );

		meshes.push_back( mesh );
	}

	return meshes;
}

#endif //_VRML_LOADER_CPP