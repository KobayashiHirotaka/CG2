struct TransformationMatrix 
{
	float32_t4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct VertexShaderInput 
{
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

struct VertexShaderOutput 
{
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : UV_TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput input) 
{
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.texcoord = input.texcoord;
	return output;
}