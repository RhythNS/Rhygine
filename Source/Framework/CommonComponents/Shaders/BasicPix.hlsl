// Basic shader with no lighting.

float4 main( float3 color : Color ) : SV_Target
{
	return float4( color, 0.6f );
}
