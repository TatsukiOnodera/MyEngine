cbuffer cbuff0 : register(b0)
{
	matrix mat; // ３Ｄ変換行列
	matrix matBillboard;//ビルボード行列(3_01)
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION;
	float scale : TEXCOORD;//スケール(3_03)
	float4 color : COLOR;
};

//ジオメトリーシェーダーからピクセルシェーダーへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION;//システム用頂点座標
	float2 uv : TEXCOORD;//uv値
	float4 color : COLOR;
};