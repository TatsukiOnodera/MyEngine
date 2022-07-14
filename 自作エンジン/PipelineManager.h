#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class PipelineManager
{
public: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //メンバ変数
	//パイプラインステート
	ComPtr<ID3D12PipelineState> m_pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> m_rootSignature;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dev">デバイス</param>
	PipelineManager(ID3D12Device* dev);

	/// <summary>
	/// ADSシェーダー
	/// </summary>
	/// <param name="dev">デバイス</param>
	void CreateADSPipeline(ID3D12Device* dev);
	
	/// <summary>
	/// トゥーンシェーダー
	/// </summary>
	/// <param name="dev">デバイス</param>
	void CreateToonPipeline(ID3D12Device* dev);

	/// <summary>
	/// 単色シェーダー
	/// </summary>
	/// <param name="dev">デバイス</param>
	void CreateMonochromaticPipeline(ID3D12Device* dev);

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	ID3D12PipelineState *GetPipelineState() { return m_pipelineState.Get(); }

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	ID3D12RootSignature *GetRootSignature() { return m_rootSignature.Get(); }
};
