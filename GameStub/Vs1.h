struct OUT{
	float4 pos:POSITION0;
	float3 light[NUM_DIRECTIONAL+1]:COLOR;
	float2 t0:TEXCOORD0;
};
struct IN{
	float4 pos:POSITION0;
	float3 n:NORMAL0;
	float3 b:BINORMAL0;
	float3 t:TANGENT0;
	float2 t0:TEXCOORD0;
};
//float4x4 mViewProj;
float4x4 mWorldViewProj;
//float4x4 mWorld;
float4 DiffuseMaterial;
float4 SpecularMaterial;
float3 DiffuseDirectional[NUM_DIRECTIONAL];
float3 SpecularDirectional[NUM_DIRECTIONAL];
float3 LightVectorDirectional[NUM_DIRECTIONAL];
//float3 HalfVectorDirectional[MAX_NUM_DIRECTIONAL];
float3 CamPosition;
float3 Ambient;
OUT Main(IN _in){
	OUT _out;
	_out.pos=mul(_in.pos,mWorldViewProj);
//	float3x3 oSp2tSp={ - _in.n.z,	_in.n.y,	_in.n.x,
//					   -_in.n.y,	_in.n.x,	_in.n.z,
//					   _in.n.x,		_in.n.y,	_in.n.z};
//	float3x3 oSp2tSp={ - _in.n.z,	-_in.n.y,	_in.n.x,
//					   _in.n.y,		_in.n.x,	_in.n.y,
//					   _in.n.x,		_in.n.z,	_in.n.z};
//	float3x3 oSp2tSp={-_in.n.y,	 - _in.n.z,		_in.n.x,
//					  _in.n.x,	 _in.n.y,		_in.n.y,
//					  _in.n.z,	 _in.n.x,		_in.n.z};
	//float3 light[NUM_DIRECTIONAL+1]:COLOR;
	for(int i=0;i<NUM_DIRECTIONAL;++i){
//		float3 light:register(vs_3_0,o2[i]);
		float3 t={dot(_in.t,LightVectorDirectional[i]),dot(_in.b,LightVectorDirectional[i]),dot(_in.n,LightVectorDirectional[i])};
		//light=normalize(t);
		_out.light[i]=normalize(t);
	}
	{
		float3 tt=CamPosition - _in.pos.xyz;
		float3 t={dot(_in.t,tt),dot(_in.b,tt),dot(_in.n,tt)};
//		float3 light:register(vs_3_0,o[NUM_DIRECTIONAL+2]);
		//light=normalize(t);
		_out.light[NUM_DIRECTIONAL]=normalize(t);
	}
	//_out.diffuse.w=DiffuseMaterial.w;
	//_out.specular.w=SpecularMaterial.w;
	_out.t0=_in.t0;
	return _out;
}

