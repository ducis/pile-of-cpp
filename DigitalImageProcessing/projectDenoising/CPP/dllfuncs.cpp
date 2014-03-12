#include "dllfuncs.h"

#include <fstream>
#include <algorithm>
#include "funcs.h"


using namespace std;


extern "C" {
	__declspec(dllexport) void TestPrintMatrixf(unsigned long nRow, unsigned long nCol, const float *m){
		wofstream os(L"TestPrintMatrixf.txt");
		const float *p(m);
		for(unsigned long r(0);r<nRow;++r){
			for(unsigned long c(0);c<nCol;++c){
				os<<*p++;
			}
			os<<endl;
		}
	}
	__declspec(dllexport) void TestMat(unsigned long nRow, unsigned long nCol, float *out){
		//fill(out, out+nRow*nCol, 4.0f);
		unsigned long n( nRow * nCol );
		for( unsigned long i(0); i<n; ++i ){
			out[i] = i;
		}
	}
	__declspec(dllexport) float *TestFours(unsigned long nRow, unsigned long nCol){//wrong
		float *out(new float[nRow*nCol]);
		fill(out, out+nRow*nCol, 4.0f);
		return out;
	}
	__declspec(dllexport) void NonLocalDenoisef(
		float *out,
		unsigned long nRowImg, unsigned long nColImg, const float *img,
		unsigned long nRowKernel, unsigned long nColKernel, const float *kernel,
		long minR, long maxR, long minC, long maxC, long rFind, long rSim, float oneOverHSquared
		)
	{
		const Matrix<float> m( NonLocalDenoise(Matrix<float>(nRowImg,nColImg,img),minR-1,maxR,minC-1,maxC,rFind,rSim,Matrix<float>(nRowKernel,nColKernel,kernel),oneOverHSquared) );
		copy(m.Begin(),m.End(),out);
	}
	__declspec(dllexport) void NonLocalDenoised(
		double *out,
		unsigned long nRowImg, unsigned long nColImg, const double *img,
		unsigned long nRowKernel, unsigned long nColKernel, const double *kernel,
		long minR, long maxR, long minC, long maxC, long rFind, long rSim, double oneOverHSquared
		)
	{
		const Matrix<double> m( NonLocalDenoise(Matrix<double>(nRowImg,nColImg,img),minR-1,maxR,minC-1,maxC,rFind,rSim,Matrix<double>(nRowKernel,nColKernel,kernel),oneOverHSquared) );
		copy(m.Begin(),m.End(),out);
	}
}
