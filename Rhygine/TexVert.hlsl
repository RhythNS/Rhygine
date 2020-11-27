// Shader for displaying an unlit texture.

cbuffer CBuf
{
    matrix transform; // perspective matrix
};

struct VSOut
{
    float2 texCoord : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float2 texCoord : TEXCOORD)
{
    VSOut vso;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vso.pos = mul(float4(pos, 1.0f), transform);
    // pass through uv coordinates
    vso.texCoord = texCoord;
    return vso;
}