sampler2D kernel;
sampler2D originalSignal;
float4 main(float2 uv:TEXCOORD0):COLOR
{
	float4 k = tex2D(kernel,uv);
	float4 m = tex2Dlod(kernel,float4(0.5,0.5,0,12));
	float4 o = tex2D(originalSignal, uv);
	float4 s = float4( (k.xyz-m.xyz/m.w)*k.w, k.w );
	return s*o;
}
