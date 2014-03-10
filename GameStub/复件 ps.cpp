struct OUT{
	float4 c:COLOR0;
//	float d:DEPTH;
};
struct IN{
	float4 c0:COLOR0;
	float4 c1:COLOR1;
	float4 uv0:TEXCOORD0;
	float4 vp:VPOS;
	float vface:VFACE;
};
sampler2D sDiff_2D;
sampler2D sNorm_2D;
float3 LightFactor;
OUT Entry(IN _in){
	OUT _out;
	//_out.c=_in.c0+_in.c1;//=tex2D(sDiff_2D,uv);
	//_out.c.xyz=(_in.vp.x-600)/100;
	//_out.d=0;
	_out.c=_in.c0;
	return _out;
}
