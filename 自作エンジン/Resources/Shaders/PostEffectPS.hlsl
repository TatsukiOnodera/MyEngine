#include "PostEffect.hlsli"

<<<<<<< HEAD
// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex0 : register(t0);
// 1番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = colortex1;
	}

	return float4(color.rgb, 1);
}
=======
Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	return float4(texcolor.rgb, 1);
}
>>>>>>> parent of 00f20c5... no message
