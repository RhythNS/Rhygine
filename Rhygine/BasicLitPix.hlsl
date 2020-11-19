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
    // Get the dot product of the normal and light direction.
    // if the dot product is a negative value, use 0.
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(normalize(normal), lightDir));
    
    // Ambient strength is added to angle multiplier so unlit pixels can still be seen.
    return tex.Sample(textureSampler, texCoords) * lightColor * (angleMultiplier + ambientStrength);
}