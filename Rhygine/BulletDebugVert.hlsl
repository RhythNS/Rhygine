cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 color : COLOR)
{
    VSOut vsout;
    vsout.pos = mul(float4(pos, 1.0f), transform);
    vsout.color = color;
	return vsout;
}
