cbuffer buf
{
    matrix transform;
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
    vsOut.pos = mul(float4(pos, 1.0f), transform);
    vsOut.color = color;
    vsOut.texCoord = texCoord;
    return vsOut;
}
