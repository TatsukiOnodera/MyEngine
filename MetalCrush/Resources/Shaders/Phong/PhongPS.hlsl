#include "Phong.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texColor = tex.Sample(smp, input.uv * tiling + offset) * color;

	//光沢度
	const float shininess = 30.0f;
	//頂点から視点へのベクトル
	float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
	//環境反射光
	float3 ambient = ambientColor * m_ambient;
	//シェーディングによる色
	float4 shadeColor = float4(ambient, m_alpha);

	// 平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(dirLights[i].lightDir, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-dirLights[i].lightDir + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

			// 全て加算する
			shadeColor.rgb += (diffuse + specular) * dirLights[i].lightColor;
		}
	}

	// 点光源
	for (i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			// ライトへの方向ベクトル
			float3 lightDir = pointLights[i].lightPos - input.worldpos.xyz;
			float d = length(lightDir);
			lightDir = normalize(lightDir);

			// 距離減衰係数
			float atten = 1.0f / (pointLights[i].lightAtten.x + pointLights[i].lightAtten.y * d + pointLights[i].lightAtten.z * d * d);

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightDir, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightDir + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

			// 全て加算する
			shadeColor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;
		}
	}

	// スポットライト
	for (i = 0; i < SPOTLIGHT_NUM; i++)
	{
		if (spotLights[i].active)
		{
			// ライトへの方向ベクトル
			float3 lightDir = pointLights[i].lightPos - input.worldpos.xyz;
			float d = length(lightDir);
			lightDir = normalize(lightDir);

			// 距離減衰係数
			float atten = 1.0f / (pointLights[i].lightAtten.x + pointLights[i].lightAtten.y * d + pointLights[i].lightAtten.z * d * d);
			// 角度減衰
			float cos = dot(lightDir, spotLights[i].lightDir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍、減衰終了角度の外側は0倍の輝度
			float angleAtten = smoothstep(spotLights[i].lightFactorAngleCos.y, spotLights[i].lightFactorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleAtten;

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightDir, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightDir + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

			// 全て加算する
			shadeColor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;
		}
	}

	// 丸影
	for (i = 0; i < CIRCLESHADOW_NUM; i++)
	{
		if (circleShadows[i].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterVec = circleShadows[i].casterPos - input.worldpos.xyz;
			// 投影方向での距離
			float d = dot(casterVec, circleShadows[i].dir);
			// 距離減衰係数
			float atten = saturate(1.0 / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// 距離がマイナスが0にする
			atten *= step(0, d);

			// 仮想ライト座標
			float3 lightPos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			// オブジェクト表面からライトへのベクトル
			float3 lightDir = normalize(lightPos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightDir, circleShadows[i].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍、減衰終了角度の外側は0倍の輝度
			float angleAtten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleAtten;

			// すべて減算
			shadeColor.rgb -= atten;
		}
	}

	//出力
	return shadeColor * texColor;
}