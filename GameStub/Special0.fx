struct VS_IN{
	float4 posOS:POSITION0;
	float4 normalOS:NORMAL0;
};
struct VS_OUT{
	float4 posHS:POSITION0;
	float height:COLOR0;
	float nZ:TEXCOORD0;
};
struct PS_IN{
	float2 vpos:VPOS;
	float vface:VFACE;
	float height:COLOR0;
	float nZ:TEXCOORD0;
};
struct PS_OUT{
	float4 c:COLOR0;
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
VS_OUT VertexProcess(VS_IN _in){
	VS_OUT _out;
	_out.posHS=mul(_in.posOS,mWorldViewProj);
	_out.height=mul(_in.posOS,mWorld).z;
	_out.nZ=abs(mul(_in.normalOS,mWorld).z);
	return _out;
}
PS_OUT PixelProcess(PS_IN _in){
	PS_OUT _out;
	_out.c.w=DiffuseMaterial.w;
	float t=_in.height*10.0f;
	float a=0.9f-1.0f/abs(t-floor(t)-0.5)/50.0f;
	float b=ceil(t-floor(t)-0.5);
	_out.c.xyz=a;
	return _out;
}

technique Tech0
{
    pass Pass0
    {
        VertexShader = compile vs_3_0 VertexProcess();
        PixelShader  = compile ps_3_0 PixelProcess(); 
    }
}
