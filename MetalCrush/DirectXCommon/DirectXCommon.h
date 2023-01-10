#pragma once
#include "WinApp.h"

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <vector>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

/// <summary>
/// DirectX基盤
/// </summary>
class DirectXCommon
{
public: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static DirectXCommon* GetInstance();

private: // メンバ変数
	// デバイス
	ComPtr<ID3D12Device> m_dev;
	// DXGIファクトリー
	ComPtr<IDXGIFactory6> m_dxgiFactory;
	// スワップチェーン
	ComPtr<IDXGISwapChain4> m_swapchain;
	// コマンドアロケーター
	ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
	// コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;
	// コマンドキュー
	ComPtr<ID3D12CommandQueue> m_cmdQueue;
	// レンダーターゲットビュー用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_rtvHeaps;
	// アダプターオブジェクト
	ComPtr<IDXGIAdapter1> m_tmpAdapter;
	// デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC m_descHeap{};
	// フェンス
	ComPtr<ID3D12Fence> m_fence;
	// 深度バッファ
	ComPtr<ID3D12Resource> m_depthBuffer;
	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> m_backBuffers;
	// フェンス用の値
	UINT64 m_fenceVal = 0;
	// 深度ビュー用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	// 深度値テクスチャ用
	ComPtr<ID3D12DescriptorHeap> m_depthSRVHeap;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DirectXCommon();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXCommon();

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(WinApp* app);

	/// <summary>
	/// 震度バッファのクリア
	/// </summary>
	void ClearDepth();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private: //メンバ関数
	/// <summary>
	/// デバッグレイヤーをオンにする
	/// </summary>
	void OnDebugLayer();

	/// <summary>
	/// デバイスを作成
	/// </summary>
	/// <returns>有無</returns>
	bool CreateDevice();

	/// <summary>
	/// コマンドリストを作成
	/// </summary>
	/// <returns>有無</returns>
	bool CreateCmdList();

	/// <summary>
	/// ダブルバッファの作成
	/// </summary>
	/// <returns>有無</returns>
	bool CreateDoubleBuffer(WinApp* app);

	/// <summary>
	/// フェンスの作成
	/// </summary>
	/// <returns>有無</returns>
	bool CreateFence();

	/// <summary>
	/// 深度バッファの作成
	/// </summary>
	/// <returns>有無</returns>
	bool CreateDepthBuffer();

public: // アクセッサ
	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDev() { return m_dev.Get(); }

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCmdList() { return m_cmdList.Get(); }



	/// <summary>
	/// 深度値テクスチャ用シェーダーリソースビューのGPUハンドル取得
	/// </summary>
	/// <returns>GPUハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDepthSRVHeapGPUHandle() { return m_depthSRVHeap->GetGPUDescriptorHandleForHeapStart(); }
};