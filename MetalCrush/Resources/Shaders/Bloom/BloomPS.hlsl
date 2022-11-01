#include "Bloom.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv) * color;

	// シェーディングによる色
	float4 shadecolor = texcolor;

	// 高輝度無の抽出
	float4 highlumi = shadecolor;
	float grayscale = highlumi.r * 0.299 + highlumi.g * 0.587 + highlumi.b * 0.114;
	float extract = smoothstep(0.6, 1.0, grayscale);
	highlumi = highlumi * extract;

	// ブラー
	float4 blur = float4(0, 0, 0, 0);
	float totalweight = 0;
	float _Sigma = 0.005;
	float _StepWight = 0.001;
	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWight)
	{
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWight)
		{
			float2 pickUV = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pickUV, _Sigma);
			blur += shadecolor * weight;
			totalweight += weight;
		}
	}
	blur.rgb = blur.rgb / totalweight;

	highlumi = highlumi * blur;
	
	return shadecolor + highlumi;
}