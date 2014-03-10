shared float3 AmbientLight;
shared float3 DiffuseDirectional[8];
shared float3 SpecularDirectional[8];
shared float3 LightVectorWSDirectional[8];
shared float4 CamPositionWS;
shared float4x4 mInverseWorld;
shared float4x4 mWorldViewProj;
shared float4 DiffuseMaterial;
shared float4 SpecularMaterial;
shared float3 EmissiveMaterial;
shared float4x4 mWorld;

technique UselessTechnique
{
    pass UselessPass
    {          
    }
}
