//**************************************************************//
//**************************************************************//
//*                                                            *//
//*  Copyright (C) 1998 Virtue 3D, Inc.  All Rights Reserved.  *//
//*                                                            *//
//*  File:	   VR2Vrml.h                                       *//
//*  Content:  Header file for saving a VR model as a VRML97   *//
//*            file                                            *//
//*  Date:     June 17, 1998                                   *//
//*                                                            *//
//**************************************************************//
//**************************************************************//

#include <iostream>
#include "vr.h"
#include "vrAnimationNode.h"
#include "GeomShape.h"

int VR2Vrml(const VRNode* node,char* fname);
VRNode* Vrml2VR(char* fname);
int removeEmptyShapes(VRNode* root);