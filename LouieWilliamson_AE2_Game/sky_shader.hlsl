
TextureCube texture0;
SamplerState sampler0;

cbuffer CBuffer1
{
    matrix WVPMatrix; //64 bytes
    float red_fraction; // 4 bytes
    float scale; // 4 bytes
    float2 packing; // 2 x 4 bytes = 8 bytes
} // total 80 bytes

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR; //Note the spelling of this and all instances below
    float3 texcoord : TEXCOORD;
};
// UHHH OHHHHH
VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD)
{
    VOut output;
    color.rbg = 1;
    
    //output.texcoord = texcoord;
    output.texcoord = position.xyz;
    output.position = mul(WVPMatrix, position);
    
    output.color = color;
    
    
    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
    return color * texture0.Sample(sampler0, texcoord);
}
