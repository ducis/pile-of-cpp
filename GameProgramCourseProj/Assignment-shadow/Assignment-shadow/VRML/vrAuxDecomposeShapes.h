#ifndef _DECOMPOSESHAPES_H_
#define _DECOMPOSESHAPES_H_

VRSFBool decompose(VRShape *shape, VRShape **shape1, VRShape **shape2);
void decompose_ClearMemory(void);
void uniteListOfShapes(VRShape **shapes, VRDWORD numShapes, VRShape* unitedShape);
void triangulate_ClearMemory();

#endif