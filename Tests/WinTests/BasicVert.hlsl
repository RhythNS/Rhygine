cbuffer CBuf : register(b0)
{
	float4x4 transform;
};

struct VSOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 color : Color)
{
	VSOut vso;
	vso.pos = mul(transform, float4(pos, 1.0f));
	vso.color = color;
	return vso;
}

