struct OUT{
	float4 c:COLOR0;
//	float d:DEPTH;
};
struct IN{
	float3 light[NUM_DIRECTIONAL+1]:COLOR;
	float2 t0:TEXCOORD0;
};

sampler2D sDiffuse;
sampler2D sNormal;
sampler2D sSpecular;
float4 DiffuseMaterial;
float4 SpecularMaterial;
float3 DiffuseDirectional[NUM_DIRECTIONAL];
float3 SpecularDirectional[NUM_DIRECTIONAL];
float3 LightVectorDirectional[NUM_DIRECTIONAL];
float3 AmbientLight;
//int NumDirectional;
OUT Entry(IN _in){
	OUT _out;
	float3 n=normalize(tex2D(sNormal,_in.t0).xyz-float3(0.5,0.5,0.5));
	float4 DiffuseColor=tex2D(sDiffuse,_in.t0);
	float4 SpecularColor=tex2D(sSpecular,_in.t0);
	_out.c.xyz=AmbientLight*DiffuseColor.xyz;
	SpecularColor.w*=255;
	
//	SpecularColor.xyz=1;
//	SpecularColor.w=59;
//	SpecularColor=SpecularMaterial;

	for(int i=0 ;  i < NUM_DIRECTIONAL; ++i){
		//float3 HalfVector
		float4 light=lit(dot(normalize(_in.light[i]),n),dot(normalize(_in.light[i] + _in.light[NUM_DIRECTIONAL]),n),SpecularColor.w);
		_out.c.xyz+=DiffuseColor.xyz*(light.y*DiffuseDirectional[i])+SpecularColor.xyz*(light.z*SpecularDirectional[i]);
	}
	_out.c.w=DiffuseColor.w;
	return _out;
}
