Texture2D tex;
SamplerState textureSampler;

cbuffer cBuf
{
    float4 lightColor;
    float3 lightPosition;
    float ambientStrength;
};

float4 main(float2 texCoords : TEXCOORD, float3 worldPos : POSITION, float3 normal : NORMAL) : SV_TARGET
{
    normal = normalize(normal);
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(normal, lightDir));
    
    return tex.Sample(textureSampler, texCoords) * lightColor * (angleMultiplier + ambientStrength);
}