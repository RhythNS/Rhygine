// Uses a texture, normal which is in the normal texture, light information and the camera position to light up a 3D object.
// The pixel is lit up depending on the angle of its normal and light direction.
// A specular highlight is added depending on the cameras position.

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
    // Get the normal from the normal map. Initial values are from 0.0f->1.0f so change them to -1.0f->1.0f
    float3 pixNorm = normalize(normalMap.Sample(textureSampler, texCoord).rgb * 2.0f - 1.0f);
    
    // Tangent, bitagent and normal matrix for converting to texture space.
    float3x3 tbn = transpose(float3x3(tangent, biTangent, normal));
    
    // Get the base lighting by calculating the dot product of the light pos and the normal
    // The light direction must be rotated to texture space for the normal map.
    float3 lightDir = mul(normalize(lightPosition - worldPos), tbn);
    float angleMultiplier = max(0, dot(pixNorm, lightDir));
	
    // Get the specular hightlight by the dot product of the light directions reflection and the direction to the camera.
    float3 reflectionLight = reflect(-lightDir, pixNorm);
    float3 viewDir = mul(normalize(cameraPos - worldPos), tbn);
    float specMultiplier = pow(max(0, dot(reflectionLight, viewDir)), specStrength);
    
    // Lastly put everything together
    float4 lightColorAdd = lightColor * (angleMultiplier + ambientStrength  + specMultiplier);
    return tex.Sample(textureSampler, texCoord) * lightColorAdd;
}
