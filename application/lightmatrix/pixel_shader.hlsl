
cbuffer cbPerObject : register(b0)
{
	float4x4 m4World;
	float4x4 m4View;
	float4x4 m4Projection;
	float4 colorEmissive;
};

struct VertexOut
{
    float4 f4Position : SV_Position;
    float3 f3Normal : NORMAL;
    float2 f2Texture : TEXCOORD0;
};

float4 main(VertexOut vin) : SV_Target
{
    return colorEmissive;
}
