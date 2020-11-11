struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

VSOut main( float3 pos : POSITION, float4 color : COLOR, float2 texCoord : TEXCOORD )
{
    VSOut vsOut;
    vsOut.pos = float4(pos, 1.0f);
    vsOut.color = color;
    vsOut.texCoord = texCoord;
    return vsOut;
}
