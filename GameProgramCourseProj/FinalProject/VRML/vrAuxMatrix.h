#ifndef _AUX_MATRIX_H_
#define _AUX_MATRIX_H_


class VRMatrix 	// VRMatrix: interface for the 4x4 matrix TRANSFORM class
{
public:
	VRMatrix(void);
	~VRMatrix(void);
	VRSFMat44f*	getMatrix(void);
	void		addMatrix(const VRSFMat44f mat);
	void		setMatrix(const VRSFMat44f mat);
	void		addScale(const VRSFVec3f vector);
	void		addRotation(const VRSFVec4f vector);
	void		addTranslation(const VRSFVec3f vector);
	void		vector(VRSFVec3f vec);
	void		multiply(const VRSFMat44f mat);
	void		normalize( VRSFVec3f u);
	void		rotateNormal(VRSFVec3f vec);
	VRSFMat44f* VRTransMatrix( VRTransform *vrtr);	// migrate VRTransform to VRMatrix

	VRSFMat44f	matrix;
};


#endif