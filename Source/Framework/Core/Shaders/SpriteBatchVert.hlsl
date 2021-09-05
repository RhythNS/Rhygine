// Shader for the spritebatch for drawing 2D sprites on the screen.

cbuffer buf
{
    matrix transform; // orthograpic matrix
};

struct VSOut
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : POSITION, float4 color : COLOR, float2 texCoord : TEXCOORD )
{
    VSOut vsOut;
    // pixel pos is the vertex position multiplied with the orthograpic matrix
    vsOut.pos = mul(float4(pos, 1.0f), transform);
    // pass through color
    vsOut.color = color;
    // pass through uv
    vsOut.texCoord = texCoord;
    return vsOut;
}
