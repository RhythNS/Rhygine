// Shader for the spritebatch for drawing 2D sprites on the screen.

Texture2D tex;

SamplerState textureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 main(float4 color : COLOR, float2 texCoord : TEXCOORD) : SV_TARGET
{
    return tex.Sample(textureSampler, texCoord) * color;
}
