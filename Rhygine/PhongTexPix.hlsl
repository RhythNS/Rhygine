Texture2D tex;

cbuffer CBuf
{
	float4 lightColor;
    float3 lightPosition;
    float3 cameraPos;
    float specStrength;
	float ambientStrength;
};

SamplerState textureSampler;

float4 main(float2 texCoord : Texcoord, float3 worldPos : Position, float3 normal : Normal) : SV_TARGET
{
    float3 norm = normalize(normal);
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(norm, lightDir));
	
    float3 reflectionLight = reflect(-lightDir, norm);
    float3 viewDir = normalize(cameraPos - worldPos);
    float specMultiplier = pow(max(0, dot(reflectionLight, viewDir)), specStrength);
    
    float4 lightColorAdd = lightColor * (angleMultiplier + ambientStrength + specMultiplier);
    return tex.Sample(textureSampler, texCoord) * lightColorAdd;
}