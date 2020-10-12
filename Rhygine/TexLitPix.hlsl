Texture2D tex;

cbuffer CBuf
{
	//float3 lightPosition;
	float ambientStrength;
	float4 lightColor;
    float3 lightPosition;
    float specStrength;
    float3 cameraPos;
};

SamplerState textureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 main(float2 texCoord : Texcoord, float3 worldPos : Position, float3 normal : Normal) : SV_TARGET
{
	//float4 col = lightColor * ambientStrength;
	// return tex.Sample(textureSampler, texCoord) * col;
	
    float3 norm = normalize(normal);
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(norm, lightDir));
	
    float3 reflectionLight = reflect(-lightDir, norm);
    float3 viewDir = normalize(cameraPos - worldPos);
    float specMultiplier = max(0, dot(reflectionLight, viewDir));
    
    float4 lightColorAdd = lightColor * (angleMultiplier + ambientStrength + specMultiplier * specStrength);
	
    //return tex.Sample(textureSampler, texCoord) * lightColorAdd;
	
    return lightColorAdd;

}