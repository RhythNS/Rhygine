// Uses a texture, normal and light information to light up a 3D object.
// The pixel is lit up depending on the angle of its normal and the light direction.

cbuffer CBuf
{
    matrix transform; // perspective matrix
    matrix worldPos; // matrix for scale, rotation, translation
    matrix localScaleRotation; // matrix for scale, rotation
};

struct VSOut
{
    float2 texCoords : TEXCOORD;
    float3 worldPos : Position;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float3 normal : NORMAL, float2 texCoords : TEXCOORD)
{
    VSOut vso;
    // pass through uv coordinates
    vso.texCoords = texCoords;
    // Normal needs to be rotated
    vso.normal = mul(float4(normal, 1.0f), localScaleRotation).xyz;
    // worldpos is the vertex position multiplied by the scale, rotation and translation
    vso.worldPos = mul(float4(pos, 1.0f), worldPos).xyz;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vso.pos = mul(float4(pos, 1.0f), transform);
    return vso;
}
