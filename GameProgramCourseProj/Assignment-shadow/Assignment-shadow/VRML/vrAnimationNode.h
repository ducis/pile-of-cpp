//**************************************************************//
//**************************************************************//
//*                                                            *//
//*  Copyright (C) 1999 Virtue 3D, Inc.  All Rights Reserved.  *//
//*                                                            *//
//*  File:	   vrAnimationNode.h                               *//
//*  Content:  VR animation nodes header file                  *//
//*  Date:     February 17, 1999                               *//
//*                                                            *//
//**************************************************************//
//**************************************************************//

#ifndef  _VR_ANIMATION_NODE_
#define  _VR_ANIMATION_NODE_

#include "vrField.h"
#include "vrNode.h"

class VRCoordinateInterpolator;
class VROrientationInterpolator;
class VRPositionInterpolator;
class VRTimeSensor;
class VRRoute;



class VRInterpolator: public VRNode
{
public:
	VRInterpolator(void);
	~VRInterpolator(){};
	virtual void release(void);
	void Copy(VRInterpolator*);

	VRMFFloat key;
};


class VRCoordinateInterpolator: public VRInterpolator
{
public:
	VRCoordinateInterpolator(void);
	~VRCoordinateInterpolator(){};
	virtual void release(void);
	void Copy(VRCoordinateInterpolator*);

	void applyIndices(); // removes indices and replaces key values

	VRM2FVec3f keyValue;
	VRMFInt32 keyValueIndex;
};

class VROrientationInterpolator: public VRInterpolator
{
public:
	VROrientationInterpolator(void);
	~VROrientationInterpolator(){};
	virtual void release(void);
	void Copy(VROrientationInterpolator*);

	VRMFRotation keyValue;
};

class VRPositionInterpolator: public VRInterpolator
{
public:
	VRPositionInterpolator(void);
	~VRPositionInterpolator(){};
	virtual void release(void);
	void Copy(VRPositionInterpolator*);

	VRMFVec3f keyValue;
};

class VRSensor: public VRNode
{
public:
	VRSensor(void);
	~VRSensor(){};

	VRSFBool enabled;
};

class VRTimeSensor: public VRSensor
{
public:
	VRTimeSensor(void);
	~VRTimeSensor(){};
	virtual void release(void);
	void Copy(VRTimeSensor*);


	VRSFTime cycleInterval;
	VRSFBool loop;
	VRSFTime startTime;
	VRSFTime stopTime;
};


typedef enum {
	VR_NO_EVENT_OUT,
	VR_FRACTION_CHANGED, VR_VALUE_CHANGED, 
} VREventOutType;

typedef enum {
	VR_NO_EVENT_IN,
	VR_SET_FRACTION, 
	VR_SET_TRANSFORM_TRANSLATION, VR_SET_TRANSFORM_SCALE, 
	VR_SET_TRANSFORM_CENTER, VR_SET_TRANSFORM_ROTATION,
	VR_SET_TRANSFORM_SCALE_ORIENTATION,
	VR_SET_COORDINATE_POINT,
	VR_SET_VIEWPOINT_POSITION, VR_SET_VIEWPOINT_ORIENTATION
} VREventInType;

class VRRoute: public VRNode
{
public:
	VRRoute(void);
	virtual void release(void);
	void Copy(VRRoute*);

	VRSFNode from;
	VRSFNode to;

	VREventOutType eventFrom;
	VREventInType  eventTo;
};


#endif	// _VR_ANIMATION_NODE_
