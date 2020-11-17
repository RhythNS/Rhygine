cbuffer CBuf
{
    matrix transform;
    matrix worldPos;
    matrix localScaleRotation;
};

struct VSOut
{
    float2 texCoord : Texcoord;
    float3 worldPos : Position;
    float3 tangent : Tangent;
    float3 biTangent : Binormal;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 tangent : TANGENT, float3 normal : BINORMAL, float2 texCoord : Texcoord)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.texCoord = texCoord;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), worldPos);
    vso.tangent = normalize(mul(float4(tangent, 1.0f), localScaleRotation));
    vso.biTangent = normalize(mul(float4(cross(tangent, normal), 1.0f), localScaleRotation));
    vso.normal = normalize(mul(float4(normal, 1.0f), localScaleRotation));
    return vso;
}