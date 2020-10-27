cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VSOut main(float3 pos : Position, float4 color : COLOR)
{
    VSOut vsout;
    vsout.color = color;
    vsout.pos = mul(float4(pos, 1.0f), transform);
	return vsout;
}
