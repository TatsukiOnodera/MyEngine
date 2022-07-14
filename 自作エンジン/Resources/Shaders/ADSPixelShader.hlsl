#include "ADSShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	//シェーディングによる色
	float4 shadecolor;
	//光沢度
	const float shininess = 30.0f;
	//頂点から視点へのベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//ハーフベクトル
	float3 halfvec = normalize(lightv + eyedir);
	//ライトに向かうベクトルと法線の内積
	float intensity = saturate(dot(normalize(input.normal), halfvec));
	//環境反射光
	float3 ambient = m_ambient;
	//拡散反射光
	float3 diffuse = m_diffuse * intensity;
	//鏡面反射光
	float3 specular = m_specular * pow(intensity, shininess);
	//すべて加算
	shadecolor.rgb = (ambient + diffuse + specular);
	shadecolor.a = m_alpha;
	texcolor.rgb *= lightcolor;

	return shadecolor * texcolor;
}