struct VS_OUT{
	float4 pos:POSITION0;
	//float3 light[NUM_DIRECTIONAL+1]:COLOR;//´ýÓÅ»¯
	float3 normal:COLOR0;
	float3 tangent:COLOR1;
	float3 binormal:COLOR2;
	float3 posOS:COLOR3;
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
	float3 normal:COLOR0;
	float3 tangent:COLOR1;
	float3 binormal:COLOR2;
	float3 posOS:COLOR3;
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
shared float4x4 mWorld;
sampler2D sDiffuse;
sampler2D sNormal;
sampler2D sSpecular;
VS_OUT PixelLighting_VS(VS_IN _in){
	float3 CamPosOS=mul(CamPositionWS,mInverseWorld).xyz;
	VS_OUT _out;
	_out.pos=mul(_in.pos,mWorldViewProj);
	_out.normal=_in.n;
	_out.binormal=_in.b;
	_out.tangent=_in.t;
	_out.posOS=_in.pos.xyz;
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
	if(!bSpecularMaterial)
		SpecularColor.w*=255;
	float3 LightVectorOSDirectional[8];
	for(int i=0;	i< NUM_DIRECTIONAL;++i){
		LightVectorOSDirectional[i]=normalize(mul(LightVectorWSDirectional[i],(float3x3)mInverseWorld));
	}
	float3 CamPositionOS=mul(CamPositionWS,mInverseWorld).xyz;
	float3 CamVectorOS=normalize(CamPositionOS-_in.posOS);
	float3 nBase=normalize(_in.normal);
	float3 bBase=normalize(_in.binormal);
	float3 tBase=normalize(_in.tangent);
	float3 CamVecTS={dot(CamPositionOS,tBase),dot(CamPositionOS,bBase),dot(CamPositionOS,nBase)};
	for(int i=0 ;  i < NUM_DIRECTIONAL; ++i){
		float3 LtVecTS={dot(LightVectorOSDirectional[i],tBase),dot(LightVectorOSDirectional[i],bBase),dot(LightVectorOSDirectional[i],nBase)};
		float3 HalfVecTS=normalize(LtVecTS+CamVecTS);
		float4 light=lit(dot(LtVecTS,n),dot(HalfVecTS,n),SpecularColor.w);
		//float4 light=lit(dot(LightVectorOSDirectional[i],nBase),dot(normalize(LightVectorOSDirectional[i]+CamVectorOS),nBase),SpecularColor.w);
		_out.c.xyz+=DiffuseColor.xyz*(light.y*DiffuseDirectional[i])+SpecularColor.xyz*(light.z*SpecularDirectional[i]);
		//_out.c.xyz=light.z;//test
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
