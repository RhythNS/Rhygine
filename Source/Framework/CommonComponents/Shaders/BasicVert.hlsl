// Basic shader with no lighting.

cbuffer CBuf
{
    matrix transform; // perspective matrix
};

struct VSOut
{
    float3 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 color : Color)
{
    VSOut vso;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vso.pos = mul(float4(pos, 1.0f), transform);
    // pass through color
    vso.color = color;
    return vso;
}
