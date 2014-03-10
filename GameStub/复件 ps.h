struct OUT{
	float4 c:COLOR0;
//	float d:DEPTH;
};
struct IN{
	float4 n:COLOR0;
	float4 camVec:COLOR1;
	float4 uv0:TEXCOORD0;
};
sampler2D sDiffuse;
sampler2D sNormal;
sampler2D sSpecular;
float3 DiffuseDirectional[NUM_DIRECTIONAL];
float3 SpecularDirectional[NUM_DIRECTIONAL];
float3 LightVectorDirectional[NUM_DIRECTIONAL];
float3 AmbientLight;
int NumDirectional;
OUT Entry(IN _in){
	OUT _out;
	_out.c.xyz=AmbientLight.xyz;
	float3 n=_in.n.xyz;
	float4 DiffuseColor=tex2D(sDiffuse,_in.uv0);
	float4 SpecularColor=tex2D(sSpecular,_in.uv0);
	SpecularColor.w*=10;
	for(int i=0 ;  i < NumDirectional ; ++i){
		//float3 HalfVector
		float4 light=lit(dot(LightVectorDirectional[i],n),dot(normalize(_in.camVec + LightVectorDirectional[i]),n),SpecularColor.w);
		_out.c.xyz+=DiffuseColor.xyz*(light.y*DiffuseDirectional[i])+SpecularColor.xyz*(light.z*SpecularDirectional[i]);
	}
	_out.c.w=DiffuseColor.w;
	return _out;
}
