cbuffer CBuf
{
    matrix transform;
    matrix worldPos;
    matrix localScaleRotation;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float2 texCoord : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD)
{
    VSOut vsout;
    vsout.texCoord = texCoord;
    vsout.worldPos = mul(float4(pos, 1.0f), worldPos);
    vsout.normal = mul(float4(normal, 1.0f), localScaleRotation);
    vsout.pos = mul(float4(pos, 1.0f), transform);
	return vsout;
}