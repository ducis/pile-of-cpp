#ifndef _VRTREEVAL_H_
#define _VRTREEVAL_H_
#include "GeomShape.h"

#define NAME_LEN 256

typedef union{
	char id[NAME_LEN];
	int int32;
	float float_val;
	VRSFTime time_val;
	float vec2f[2];
	float vec3f[3];
	float vec4f[4];
	VRSFBool sf_bool;
	VRNode* sf_node;
	VRMFInt32* mf_int32;
	VRMFFloat* mf_float;
	VRMFVec2f* mf_vec2f;
	VRMFVec3f* mf_vec3f;
	VRMFRotation* mf_rotation;
	VRMFString* mf_string;
	VRMFColor* mf_color;
	VRMFNode* mf_node;
} WRLSTYPE;

#endif