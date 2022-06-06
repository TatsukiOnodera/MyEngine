#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
protected:
	//テクスチャリソース（テクスチャバッファ）
	ComPtr<ID3D12Resource> texBuff;
	//テクスチャ用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画コマンド
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
};