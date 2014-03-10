struct VS_OUT{
	float4 pos:POSITION0;
	float3 light[NUM_DIRECTIONAL+1]:COLOR;//´ýÓÅ»¯
	float2 t0:TEXCOORD0;
};
struct VS_IN{
	float4 pos:POSITION0;
	float3 n:NORMAL0;
	float3 b:BINORMAL0;
	float3 t:TANGENT0;
	float2 t0:TEXCOORD0;
};
struct PS_OUT{
	float4 c:COLOR0;
//	float d:DEPTH;
};
struct PS_IN{
	float3 light[NUM_DIRECTIONAL+1]:COLOR;
	float2 t0:TEXCOORD0;
};
shared float3 AmbientLight;
shared float3 DiffuseDirectional[8];
shared float3 SpecularDirectional[8];
shared float3 LightVectorWSDirectional[8];
shared float4 CamPositionWS;
shared float4x4 mInverseWorld;
shared float4x4 mWorldViewProj;
shared float4 DiffuseMaterial;
shared float4 SpecularMaterial;
//shared float3 AmbientMaterial;
shared float3 EmissiveMaterial;
sampler2D sDiffuse;
sampler2D sNormal;
sampler2D sSpecular;
VS_OUT PixelLighting_VS(VS_IN _in){
	float3 CamPosOS=mul(CamPositionWS,mInverseWorld).xyz;
	VS_OUT _out;
	_out.pos=mul(_in.pos,mWorldViewProj);
	for(int i=0;i<NUM_DIRECTIONAL;++i){
		float3 LtVecOSDirectional=normalize(mul(LightVectorWSDirectional[i].xyz,(float3x3)mInverseWorld));
		//float3 LtVecOSDirectional=LightVectorWSDirectional[i];
		//float3 t={dot(_in.t,LightVectorWSDirectional[i]),dot(_in.b,LightVectorWSDirectional[i]),dot(_in.n,LightVectorWSDirectional[i])};
		//_out.light[i]=normalize(float3(dot(_in.t,LtVecOSDirectional),dot(_in.b,LtVecOSDirectional),dot(_in.n,LtVecOSDirectional)));
		_out.light[i]=normalize(float3(dot(_in.t,LtVecOSDirectional),dot(_in.b,LtVecOSDirectional),dot(_in.n,LtVecOSDirectional)));
	}
	{
		float3 tt=CamPosOS - _in.pos.xyz;
		_out.light[NUM_DIRECTIONAL]=normalize(float3(dot(_in.t,tt),dot(_in.b,tt),dot(_in.n,tt)));
	}
	_out.t0=_in.t0;
	return _out;
}
PS_OUT PixelLighting_PS(PS_IN _in,uniform bool bDiffuseMaterial,uniform bool bDiffuseMap,uniform bool bSpecularMaterial,uniform bool bSpecularMap,uniform bool bNormalMap){
	PS_OUT _out;
	float3 n=bNormalMap?normalize(tex2D(sNormal,_in.t0).xyz-float3(0.5,0.5,0.5)):float3(0,0,1);
	float4 DiffuseColor=bDiffuseMaterial?
		(bDiffuseMap?tex2D(sDiffuse,_in.t0)*DiffuseMaterial:DiffuseMaterial):
		(bDiffuseMap?tex2D(sDiffuse,_in.t0):float4(1,1,1,1));
	float4 SpecularColor=bSpecularMaterial?
		(bSpecularMap?tex2D(sSpecular,_in.t0)*SpecularMaterial:SpecularMaterial):
		(bSpecularMap?tex2D(sSpecular,_in.t0):float4(0,0,0,0));
	_out.c.xyz=AmbientLight*DiffuseColor.xyz;
	//_out.c.xyz=0;//test
	if(!bSpecularMaterial)
		SpecularColor.w*=255;
	for(int i=0 ;  i < NUM_DIRECTIONAL; ++i){
		//float3 HalfVector
		float4 light=lit(dot(normalize(_in.light[i]),n),dot(normalize(_in.light[i] + _in.light[NUM_DIRECTIONAL]),n),SpecularColor.w);
		//_out.c.xyz+=DiffuseColor.xyz*(light.y*DiffuseDirectional[i])+SpecularColor.xyz*(light.z*SpecularDirectional[i]);
		_out.c.xyz=light.z;//test
	}
	_out.c.w=DiffuseColor.w;
	//_out.c.xyz=DiffuseDirectional[0];
	//_out.c=DiffuseMaterial;//test
	return _out;
}
technique PixelLighting
{
    pass P0
    {          
        VertexShader = compile vs_3_0 PixelLighting_VS();
        PixelShader  = compile ps_3_0 PixelLighting_PS(true,false,true,false,false); 
    }
}
