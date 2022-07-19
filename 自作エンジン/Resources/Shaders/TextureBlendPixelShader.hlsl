#include "ADSShaderHeader.hlsli"

Texture2D<float4> maintex : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> subtex : register(t1);  // 1番スロットに設定されたテクスチャ
Texture2D<float4> masktex : register(t2);  // 2番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 maincolor = maintex.Sample(smp, input.uv) * color;
	float4 subcolor = subtex.Sample(smp, input.uv) * color;
	float4 maskcolor = masktex.Sample(smp, input.uv);

	//シェーディングによる色
	float4 shadecolor = maskcolor.r * subcolor + (1 - maskcolor.r) * maincolor;

	//出力
	return shadecolor;
}