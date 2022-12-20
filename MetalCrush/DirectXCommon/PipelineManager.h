#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>

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
	/// <param name="shaderName">シェーダー名</param>
	PipelineManager(ID3D12Device* dev, std::wstring shaderName);

	/// <summary>
	/// フォンシェーダー
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="shaderName">シェーダー名</param>
	void CreateShaderPipeline(ID3D12Device* dev, std::wstring shaderName);

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	ID3D12PipelineState *GetPipelineState() { return m_pipelineState.Get(); }

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	/// <returns>ルートシグネチャ</returns>
	ID3D12RootSignature *GetRootSignature() { return m_rootSignature.Get(); }
};
