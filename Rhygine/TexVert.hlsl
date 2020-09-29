cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float2 texCoord : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float2 texCoord : TEXCOORD)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.texCoord = texCoord;
    return vso;
}