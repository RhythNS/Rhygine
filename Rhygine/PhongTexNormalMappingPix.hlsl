Texture2D tex : register(t0);
Texture2D normalMap : register(t1);

cbuffer CBuf
{
    float4 lightColor;
    float3 lightPosition;
    float3 cameraPos;
    float specStrength;
    float ambientStrength;
};

SamplerState textureSampler;

float4 main(float2 texCoord : Texcoord, float3 worldPos : Position, float3 tangent : Tangent, float3 biTangent : Binormal, float3 normal : NORMAL) : SV_TARGET
{
    float3 pixNorm = normalize(normalMap.Sample(textureSampler, texCoord).rgb * 2.0f - 1.0f);
    float3x3 tbn = transpose(float3x3(tangent, biTangent, normal));
    
    float3 lightDir = mul(normalize(lightPosition - worldPos), tbn);
    float angleMultiplier = max(0, dot(pixNorm, lightDir));
	
    float3 reflectionLight = reflect(-lightDir, pixNorm);
    float3 viewDir = mul(normalize(cameraPos - worldPos), tbn);
    float specMultiplier = pow(max(0, dot(reflectionLight, viewDir)), specStrength);
    
    float4 lightColorAdd = lightColor * (angleMultiplier + ambientStrength  + specMultiplier);
    return tex.Sample(textureSampler, texCoord) * lightColorAdd;
}