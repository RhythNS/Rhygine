Texture2D tex;

cbuffer CBuf
{
    float3 lightPosition[10] : POSITION;
    float3 lightDirection[10] : NORMAL;
    uint lightAmount; 
};

SamplerState textureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 main(float2 texCoord : TEXCOORD, float4 normal : NORMAL) : SV_TARGET
{
    return tex.Sample(textureSampler, texCoord);
}