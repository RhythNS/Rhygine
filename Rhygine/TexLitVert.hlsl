cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float2 texCoord : Texcoord;
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 texCoord : Texcoord, float3 normal : Normal)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.texCoord = texCoord;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), transform);
    vso.normal = mul(float4(normal, 1.0f), transform);
	return vso;
}