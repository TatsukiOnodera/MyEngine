cbuffer cbuff0 : register(b0)
{
	matrix viewproj; //ビュープロジェクション行列
	matrix world; //ワールド行列
	float3 cameraPos; //カメラ座標
};

//ボーン最大数
static const int MAX_BONES = 32;

cbuffer skinning : register(b3)
{
	matrix matSkinning[MAX_BONES]; //ボーンスキニング行列が入る
};

//バーテックスバッファの入力
struct VSInput
{
	float4 pos : POSITION; //座標
	float3 normal : NORMAL; //頂点法線
	float2 uv : TEXCOORD; //テクスチャ座標
	uint4 boneIndices : BONEINDICES; //ボーン番号
	float4 boneWeights : BONEWEIGHTS; //ボーンスキンウェイト
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal : NORMAL; // 法線ベクトル
	float2 uv  : TEXCOORD; // uv値
};
