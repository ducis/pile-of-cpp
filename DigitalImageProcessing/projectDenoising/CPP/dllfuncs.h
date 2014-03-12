#pragma once

#ifdef __cplusplus
extern "C"{
#endif

	__declspec(dllexport) void TestPrintMatrixf(unsigned long nRow, unsigned long nCol, const float *m);
	__declspec(dllexport) void TestMat(unsigned long nRow, unsigned long nCol, float *out);
	__declspec(dllexport) float *TestFours(unsigned long nRow, unsigned long nCol);
	__declspec(dllexport) void NonLocalDenoisef(
		float *out,
		unsigned long nRowImg, unsigned long nColImg, const float *img,
		unsigned long nRowKernel, unsigned long nColKernel, const float *kernel,
		long minR, long maxR, long minC, long maxC, long rFind, long rSim, float oneOverHSquared
		);
	__declspec(dllexport) void NonLocalDenoised(
		double *out,
		unsigned long nRowImg, unsigned long nColImg, const double *img,
		unsigned long nRowKernel, unsigned long nColKernel, const double *kernel,
		long minR, long maxR, long minC, long maxC, long rFind, long rSim, double oneOverHSquared
		);
#ifdef __cplusplus
}
#endif
