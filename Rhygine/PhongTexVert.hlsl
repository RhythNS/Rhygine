// Uses a texture, normal, light information and the camera position to light up a 3D object.
// The pixel is lit up depending on the angle of its normal and light direction.
// A specular highlight is added depending on the cameras position.

cbuffer CBuf
{
    matrix transform; // perspective matrix
    matrix worldPos; // matrix for scale, rotation, translation
    matrix localScaleRotation; // matrix for scale, rotation
};

struct VSOut
{
    float2 texCoord : Texcoord;
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 normal : Normal, float2 texCoord : Texcoord)
{
    VSOut vso;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vso.pos = mul(float4(pos, 1.0f), transform);
    // pass through uv coordinates
    vso.texCoord = texCoord;
    // worldpos is the vertex position multiplied by the scale, rotation and translation
    vso.worldPos = (float3) mul(float4(pos, 1.0f), worldPos);
    // Normal needs to be rotated
    vso.normal = mul(float4(normal, 1.0f), localScaleRotation).xyz;
    return vso;
}