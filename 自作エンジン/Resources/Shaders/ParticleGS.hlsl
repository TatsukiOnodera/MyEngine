#include "Particle.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからオフセット
static const float4 offset_array[vnum] = {
	float4(-0.5f,-0.5f,0,0),
	float4(-0.5f,+0.5f,0,0),
	float4(+0.5f,-0.5f,0,0),
	float4(+0.5f,+0.5f,0,0)
};

//左上が0,0   右下が1,1
static const float2 uv_array[vnum] = {
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0),
};

//点の入力から、四角形を出力
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	//点ストリーム
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//4点分回す
	for (uint i = 0; i < vnum; i++)
	{
		//中心からオフセットをスケーリング(3_03)
		float4 offset = offset_array[i] * input[0].scale;
		//中心からのオフセットをビルボード回転(モデル座標)(3_03)
		offset = mul(matBillboard, offset);
		//オフセット分ずらす(ワールド座標)
		element.svpos = input[0].pos + offset;
		//ビュー、射影変換
		element.svpos = mul(mat,element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}