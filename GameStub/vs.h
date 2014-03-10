struct OUT{
	float4 pos:POSITION0;
	float4 diffuse:COLOR0;
	float4 specular:COLOR1;
	float2 t0:TEXCOORD0;
};
struct IN{
	float4 pos:POSITION0;
	float4 n:NORMAL;
	float2 t0:TEXCOORD0;
};
float4x4 mViewProj;
float4x4 mWorldViewProj;
float4x4 mWorld;
float4 DiffuseMaterial;
float4 SpecularMaterial;
float3 DiffuseDirectional[MAX_NUM_DIRECTIONAL];
float3 SpecularDirectional[MAX_NUM_DIRECTIONAL];
float3 LightVectorDirectional[MAX_NUM_DIRECTIONAL];
//float3 HalfVectorDirectional[MAX_NUM_DIRECTIONAL];
float3 CamPosition;
float3 Ambient;
int NumDirectional;
OUT Main(IN _in){
	OUT _out;
	_out.pos=mul(_in.pos,mWorldViewProj);
	_out.diffuse.xyz=Ambient.xyz;
	_out.specular.xyz=0;
	float3 CamVector=normalize(CamPosition - _in.pos);
	float3 n={mul(_in.n,mWorld[0]),mul(_in.n,mWorld[1]),mul(_in.n,mWorld[2])};
	for(int i=0 ;  i < NumDirectional ; ++i){
		//float3 HalfVector
		float4 light=lit(dot(LightVectorDirectional[i],n),dot(normalize(CamVector + LightVectorDirectional[i]),n),SpecularMaterial.w);
		_out.diffuse.xyz+=light.y*DiffuseMaterial*DiffuseDirectional[i];
		_out.specular.xyz+=light.z*SpecularMaterial*SpecularDirectional[i];
	}
	_out.diffuse.w=DiffuseMaterial.w;
	_out.specular.w=SpecularMaterial.w;
	_out.t0=_in.pos.xy;
	return _out;
}
