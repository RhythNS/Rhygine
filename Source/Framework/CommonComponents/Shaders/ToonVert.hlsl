// Uses a texture, normal and light position to light up a pixel.
// The effect shows clear lighting steps to emulate a drawn or cartoon effect.
// The lighting steps can be changed in the cbuffer on the PixShaders as the variable "levels".

cbuffer CBuf
{
    matrix transform; // perspective matrix
    matrix worldPos; // matrix for scale, rotation, translation
    matrix localScaleRotation; // matrix for scale, rotation
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float2 texCoord : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD)
{
    VSOut vsout;
    // pass through uv coordinates
    vsout.texCoord = texCoord;
    // worldpos is the vertex position multiplied by the scale, rotation and translation
    vsout.worldPos = mul(float4(pos, 1.0f), worldPos).xyz;
    // Normal needs to be rotated
    vsout.normal = mul(float4(normal, 1.0f), localScaleRotation).xyz;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vsout.pos = mul(float4(pos, 1.0f), transform);
	return vsout;
}
