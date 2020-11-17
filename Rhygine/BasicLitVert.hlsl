cbuffer CBuf
{
    matrix transform;
    matrix worldPos;
    matrix localScaleRotation;
};

struct VSOut
{
    float2 texCoords : TEXCOORD;
    float3 worldPos : Position;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float3 normal : NORMAL, float2 texCoords : TEXCOORD)
{
    VSOut vso;
    vso.texCoords = texCoords;
    vso.normal = mul(float4(normal, 1.0f), localScaleRotation);
    vso.worldPos = (float3) mul(float4(pos, 1.0f), worldPos);
    vso.pos = mul(float4(pos, 1.0f), transform);
    return vso;
}