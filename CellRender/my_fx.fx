
float u;
float v;

texture		g_txTeapot;
texture		g_txMark;


sampler2D g_samTeapot =
sampler_state
{
    Texture = <g_txTeapot>;
};


sampler2D g_samScene =
sampler_state
{
    Texture = <g_txMark>;
};


struct VS_OUTPUT
{
    float4 position   : POSITION;
    float2 tex : TEXCOORD;
};


VS_OUTPUT VS_MAIN(	float3 position : POSITION,
				float2 tex: TEXCOORD)
{
	VS_OUTPUT output;
	output.position = float4(position, 1.0f);
	output.tex = tex;
	
	return output;
}

float4 PS_MARK():COLOR
{
	return float4(1.0f,1.0f,1.0f,1.0f);
}

float4 PS_CARTOON(float4 diffuse : COLOR ):COLOR
{
	if(diffuse.r > 0.6f)
	{
		diffuse.r = 1.0f;
	}
	else if(diffuse.r > 0.3)
	{
		diffuse.r = 0.8f;
	}
	else
	{
		diffuse.r = 0.6f;
	}
	return diffuse;
}

float4 PS_MAIN(float2 tex : TEXCOORD):COLOR
{
	float2 temp_tex;
	temp_tex.x = tex.x - u; temp_tex.y = tex.y - v;
	float4 color1 = tex2D(g_samScene, temp_tex);
	temp_tex.x = tex.x; temp_tex.y = tex.y - v;
	float4 color2 = tex2D(g_samScene, temp_tex);
	temp_tex.x = tex.x + u; temp_tex.y = tex.y - v;
	float4 color3 = tex2D(g_samScene, temp_tex);
	
	temp_tex.x = tex.x - u; temp_tex.y = tex.y;
	float4 color4 = tex2D(g_samScene, temp_tex);
	temp_tex.x = tex.x; temp_tex.y = tex.y;
	float4 color5 = tex2D(g_samScene, temp_tex);
	temp_tex.x = tex.x + u; temp_tex.y = tex.y;
	float4 color6 = tex2D(g_samScene, temp_tex);
	
	temp_tex.x = tex.x - u; temp_tex.y = tex.y + v;
	float4 color7 = tex2D(g_samScene, temp_tex);
	temp_tex.x = tex.x; temp_tex.y = tex.y + v;
	float4 color8 = tex2D(g_samScene, temp_tex);
	temp_tex.x = tex.x + u; temp_tex.y = tex.y + v;
	float4 color9 = tex2D(g_samScene, temp_tex);
	
	float4 color0 = tex2D(g_samScene, tex);
	
	float4 temp_clr = color5*(-8)+color1+color2+color3+color4+color6+color7+color8+color9;
	

	if(temp_clr.x > 0.0f)
	{
		temp_clr = float4(1.0f,1.0f,1.0f,1.0f);
	}
	else
	{
		temp_clr = float4(0.0f,0.0f,0.0f,0.0f);
	}

	float4 clr_teapot = tex2D(g_samTeapot, tex);

	return  temp_clr + clr_teapot;
}

technique my_tech
{
    pass p0
    {
        VertexShader = compile vs_1_1 VS_MAIN();
        PixelShader = compile ps_2_0 PS_MAIN();
    }
}


technique PS_MARK
{
    pass p0
    {
        PixelShader = compile ps_2_0 PS_MARK();
    }
}

technique PS_CARTOON
{
    pass p0
    {
        PixelShader = compile ps_2_0 PS_CARTOON();
    }
}