Texture2D tex;

SamplerState textureSampler;

cbuffer lightInfo
{
    float4 lightColor;
    float3 lightPosition;
    float ambientStrength;
    int levels;
};

float4 main(float3 worldPos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) : SV_TARGET
{
    normal = normalize(normal);
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(normal, lightDir));
    
    return tex.Sample(textureSampler, texCoord) * lightColor * (ambientStrength + floor(angleMultiplier * levels) / levels);
}