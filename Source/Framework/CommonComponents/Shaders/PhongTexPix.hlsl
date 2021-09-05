// Uses a texture, normal, light information and the camera position to light up a 3D object.
// The pixel is lit up depending on the angle of its normal and light direction.
// A specular highlight is added depending on the cameras position.

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
    // Get the base lighting by calculating the dot product of the light pos and the normal.
    float3 norm = normalize(normal);
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(norm, lightDir));
    
	 // Get the specular hightlight by the dot product of the light directions reflection and the direction to the camera.
    float3 reflectionLight = reflect(-lightDir, norm);
    float3 viewDir = normalize(cameraPos - worldPos);
    float specMultiplier = pow(max(0, dot(reflectionLight, viewDir)), specStrength);
    
    // Lastly put everything together
    float4 lightColorAdd = lightColor * (angleMultiplier + ambientStrength + specMultiplier);
    return tex.Sample(textureSampler, texCoord) * lightColorAdd;
}