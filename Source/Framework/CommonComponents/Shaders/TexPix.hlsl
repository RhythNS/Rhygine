// Shader for displaying an unlit texture.

Texture2D tex;

SamplerState textureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 main(float2 texCoord : TEXCOORD) : SV_TARGET
{
    return tex.Sample(textureSampler, texCoord);
}