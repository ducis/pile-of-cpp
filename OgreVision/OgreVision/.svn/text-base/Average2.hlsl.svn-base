sampler2D a;
sampler2D b;
float4 main(float2 uv:TEXCOORD0):COLOR
{
	//return tex2Dlod(img,float4(0.01,0.01,0,1000));
	return (tex2D(a,uv)+tex2D(b,uv))*0.5;
}
