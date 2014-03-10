#include "CSceneNodeCoordSys.h"
#include "windowsnominmax.h"
#include <gl/gl.h>
void CSceneNodeCoordSys::Render() const{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	float l = m_size;
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(l,0,0);

	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,l,0);

	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,l);
	glEnd();
	glPopAttrib();
}
