Texture2D tex;

cbuffer CBuf
{
	float3 lightPosition;
	float3 lightDirection;
	float4 lightColor;
	float ambientStrength;
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
	
	return tex.Sample(textureSampler, texCoord) * ambientStrength;

}