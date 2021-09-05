// Uses a texture, normal which is in the normal texture, light information and the camera position to light up a 3D object.
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
    float3 tangent : Tangent;
    float3 biTangent : Binormal;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 tangent : TANGENT, float3 normal : BINORMAL, float2 texCoord : Texcoord)
{
    VSOut vso;
    // pixel pos is the vertex position multiplied with the perspective matrix
    vso.pos = mul(float4(pos, 1.0f), transform);
    // pass through uv coordinates
    vso.texCoord = texCoord;
    // worldpos is the vertex position multiplied by the scale, rotation and translation
    vso.worldPos = mul(float4(pos, 1.0f), worldPos).xyz;
    
    // the tangent, bitangent and normal need to be rotated
    // the bitagent can be caluclated by the cross product of the normal and tangent.
    vso.tangent = normalize(mul(float4(tangent, 1.0f), localScaleRotation)).xyz;
    vso.biTangent = normalize(mul(float4(cross(tangent, normal), 1.0f), localScaleRotation)).xyz;
    vso.normal = normalize(mul(float4(normal, 1.0f), localScaleRotation)).xyz;
    
    return vso;
}
