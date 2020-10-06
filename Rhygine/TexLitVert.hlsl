cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float2 texCoord : TEXCOORD;
    float4 normal : NORMAL;
    float4 pos : SV_Position;
};


VSOut main(float3 pos : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.normal = mul(float4(normal, 1.0f), transform);
    vso.texCoord = texCoord;
	return vso;
}