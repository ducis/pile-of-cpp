#pragma once

#ifdef __cplusplus
extern "C"{
#endif


__declspec(dllexport) void MeanShiftDPSrgbf(
	float *out,
	unsigned long nRowImg, unsigned long nColImg, const float *img,
	float spatialBandwidth, float colorBandwidth, unsigned long maxIteration, float convergenceDistSqr
	);


__declspec(dllexport) void MeanShiftSegmentationrgbf(
	unsigned long *out,
	unsigned long nRowImg, unsigned long nColImg, const float *img,
	float spatialBandwidth, float colorBandwidth, unsigned long minRegionSize, unsigned long maxIteration, float convergenceDistSqr
	);


__declspec(dllexport) void PruneSmallRegionsul(
	unsigned long *out,
	unsigned long nRowImg, unsigned long nColImg, const unsigned long *idMap,
	unsigned long minRegionSize
	);


__declspec(dllexport) void MeanShiftEdgenessMaprgbf(
	float *out,
	unsigned long nRowImg,unsigned long nColImg, const float *img,
	float spatialBandwidth,float colorBandwidth, unsigned long maxIteration, float convergenceDistSqr
	);


__declspec(dllexport) void PrintRGBf(unsigned long nRowImg, unsigned long nColImg, const float *img);
									 


#ifdef __cplusplus
}
#endif
