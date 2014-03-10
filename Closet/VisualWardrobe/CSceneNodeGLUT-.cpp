#include "CSceneNodeGLUT-.h"
#include <glut.h>
void CSceneNodeGLUTWireCube::Render() const{
	glutWireCube(1000.0f);
}
void CSceneNodeGLUTSolidCube::Render() const{
	glutSolidCube(1000.0f);
}
