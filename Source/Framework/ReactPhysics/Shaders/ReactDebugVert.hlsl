// Shader for displaying debug lines and triangles for react.

cbuffer CBuf
{
    matrix transform; // perspective matrix
};

struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 color : COLOR)
{
    VSOut vsout;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vsout.pos = mul(float4(pos, 1.0f), transform);
    // pass through color
    vsout.color = color;
	return vsout;
}
