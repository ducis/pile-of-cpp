#pragma once
#include "CViewPanner.h"
#include "3d.h"
const vector2 CViewPanner::Drag(const vector2 &displacementHS, const matrix44 &viewProj){
	float view[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view[0][0]);
	matrix44 proj;
	glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&proj);
	matrix44 projInv(proj);
	projInv.invert();
	vector4 displacementCS( projInv*vector4(displacementHS.x,displacementHS.y,0,0) );
	for(int i=0;i<3;++i){
		view[3][i]+=displacementCS[i];
	}
	glLoadMatrixf((GLfloat*)&view[0][0]);
	return displacementHS;
};
