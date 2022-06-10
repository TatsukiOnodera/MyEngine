#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public: //静的メンバ関数
	//画面クリアカラー
	static const float clearColor[4];

protected: //メンバ変数
	//テクスチャリソース（テクスチャバッファ）
	ComPtr<ID3D12Resource> texBuff[2];
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//テクスチャ用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//レンダーターゲットビュー用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//デプスステンシルビュー用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//グラフィックパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画コマンド
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
};