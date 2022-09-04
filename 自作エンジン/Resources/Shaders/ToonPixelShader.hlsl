#include "ToonShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv) * color;

	//シェーディングによる色
	float4 shadecolor;
	//光沢度
	const float shininess = 30.0;
	//頂点から視点へのベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//ハーフベクトル
	float3 halfvec = normalize(lightv + eyedir);
	//ライトに向かうベクトルと法線の内積
	float intensity = saturate(dot(normalize(input.normal), halfvec));
	//環境反射光
	float3 ambient = m_ambient;
	//smoothstep用変数
	float a_scale = 0.45;
	float b_scale = a_scale + 0.1;
	//拡散反射光
	float3 diffuse = m_diffuse * smoothstep(a_scale, b_scale, intensity);
	//鏡面反射光
	float3 specular = m_specular * smoothstep(a_scale, b_scale, pow(intensity, shininess));
	//すべて加算
	float3 ads = (ambient + diffuse + specular) * texcolor.rgb;
	//明るい部分の色
	float3 l_color = ads;
	//暗い部分の色
	float3 d_color = texcolor.rgb * 0.3;
	//トゥーン化
	shadecolor.rgb = smoothstep(a_scale, b_scale, intensity) * l_color + (1 - smoothstep(a_scale, b_scale, intensity)) * d_color * lightcolor;
	shadecolor.a = m_alpha;

	//出力
	return shadecolor;
}