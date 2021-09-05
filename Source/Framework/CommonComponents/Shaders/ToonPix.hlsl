// Uses a texture, normal and light position to light up a pixel.
// The effect shows clear lighting steps to emulate a drawn or cartoon effect.
// The lighting steps can be changed in the cbuffer on the PixShaders as the variable "levels".

Texture2D tex;

SamplerState textureSampler;

cbuffer lightInfo
{
    float4 lightColor;
    float3 lightPosition;
    float ambientStrength;
    int levels; // the amount of lighting levels.
};

float4 main(float3 worldPos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) : SV_TARGET
{
    normal = normalize(normal);
    
    // Get the base lighting by calculating the dot product of the light pos and the normal.
    float3 lightDir = normalize(lightPosition - worldPos);
    float angleMultiplier = max(0, dot(normal, lightDir));

    // To archieve a sort of "cut of" lighting we multiply the angle with the level and floor the product.
    // Now we have a value that is 1,2,3,...
    // If we divide that by the level value, then we get a value between 0 and 1. If the levels would be 4 then this value
    // can only be 0, 0.25, 0.5, 0.75, 1.
    // Now add a ambientStrength as minimum light value and multiply that by the light color and texture color.
    return tex.Sample(textureSampler, texCoord) * lightColor * (ambientStrength + floor(angleMultiplier * levels) / levels);
}
