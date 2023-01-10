#include "DirectXCommon.h"

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon dx_cmd;

	return &dx_cmd;
}

DirectXCommon::DirectXCommon()
{

}

DirectXCommon::~DirectXCommon()
{
	// 開放漏れの確認
	ID3D12DebugDevice* debugInterface;
	if (SUCCEEDED(m_dev.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}
}

bool DirectXCommon::Initialize(WinApp* app)
{
	OnDebugLayer();

	if (!(CreateDevice()))
	{
		return false;
	}
	if (!(CreateCmdList()))
	{
		return false;
	}
	if (!(CreateDoubleBuffer(app)))
	{
		return false;
	}
	if (!(CreateFence()))
	{
		return false;
	}
	if (!(CreateDepthBuffer()))
	{
		return false;
	}

	return true;
}

void DirectXCommon::OnDebugLayer()
{
	// デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void DirectXCommon::PreDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex;
	bbIndex = m_swapchain->GetCurrentBackBufferIndex();

	// 表示状態から描画状態に変更
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 描画先指定
	// レンダ―ターゲッビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		m_dev->GetDescriptorHandleIncrementSize(m_descHeap.Type)
	);

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// 深度バッファのクリア
	ClearDepth();

	// 画面クリア
	float R = 86;
	float G= 119;
	float B = 252;
	float A = 255;
	float clearColor[] = { powf((R / 255), 2.2f), powf((G / 255), 2.2f), powf((B / 255), 2.2f), A / 255 };// 青っぽい色
	m_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	// ビューポート領域の設定
	m_cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));
	// シザー矩形の設定
	m_cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));
}

void DirectXCommon::PostDraw()
{
	// リソースバリアを変更
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	m_cmdList->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { m_cmdList.Get() }; // コマンドリストの配列
	m_cmdQueue->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ（裏表の入替え）
	m_swapchain->Present(1, 0);

	// コマンドキューの実行完了を待つ
	m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
	if (m_fence->GetCompletedValue() != m_fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_fence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	m_cmdAllocator->Reset(); // キューをクリア
	m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
}

bool DirectXCommon::CreateDevice()
{
	HRESULT result = S_FALSE;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	if (result == S_FALSE)
	{
		return false;
	}

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	// ここに特定の名前を持つアダプターオブジェクトが入る
	for (int i = 0; m_dxgiFactory->EnumAdapters1(i, &m_tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(m_tmpAdapter);
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description; // アダプター名
		// inetl UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			m_tmpAdapter = adapters[i];   // 採用
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (int i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(m_tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&m_dev));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	if (result == S_FALSE)
	{
		return false;
	}

	return true;
}

bool DirectXCommon::CreateCmdList()
{
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = m_dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_cmdAllocator));
	if (result == S_FALSE)
	{
		return false;
	}

	// コマンドリストを生成
	result = m_dev->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_cmdList));
	if (result == S_FALSE)
	{
		return false;
	}

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	m_dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));

	return true;
}

bool DirectXCommon::CreateDoubleBuffer(WinApp* app)
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	ComPtr<IDXGISwapChain1> swapchain1;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	m_dxgiFactory->CreateSwapChainForHwnd(
		m_cmdQueue.Get(),
		app->GetHWND(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	swapchain1.As(&m_swapchain);

	// 各種設定をしてデスクリプタヒープを生成
	m_descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	m_descHeap.NumDescriptors = 2;    // 裏表の２つ
	m_dev->CreateDescriptorHeap(&m_descHeap,
		IID_PPV_ARGS(&m_rtvHeaps));

	// 裏表の２つ分について
	m_backBuffers.resize(m_descHeap.NumDescriptors);
	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(), //ヒープの先頭アドレス
				i, //ディスクリプタの番号
				m_dev->GetDescriptorHandleIncrementSize(m_descHeap.Type)); //ディスクリプタ一つ分のサイズ

			// レンダーターゲットビューの生成
		m_dev->CreateRenderTargetView(
			m_backBuffers[i].Get(),
			nullptr,
			handle);
		m_dev->CreateRenderTargetView(
			m_backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				m_dev->GetDescriptorHandleIncrementSize(m_descHeap.Type)
			)
		);
	}
	if (result == S_FALSE)
	{
		return false;
	}

	return true;
}

bool DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = m_dev->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_fence));
	if (result == S_FALSE)
	{
		return false;
	}

	return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// 深度バッファ
	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32_TYPELESS,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// 深度バッファの生成
	result = m_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値の書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_depthBuffer));
	if (result == S_FALSE)
	{
		return false;
	}

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = m_dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));
	if (result == S_FALSE)
	{
		return false;
	}

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_dev->CreateDepthStencilView(
		m_depthBuffer.Get(),
		&dsvDesc,
		m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	// 深度値テクスチャ用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dstHeapDesc = {};
	dstHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dstHeapDesc.NodeMask = 0;
	dstHeapDesc.NumDescriptors = 1;
	dstHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = m_dev->CreateDescriptorHeap(&dstHeapDesc, IID_PPV_ARGS(&m_depthSRVHeap));
	if (result == S_FALSE)
	{
		return false;
	}

	// 深度値テクスチャビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC dstDesc = {};
	dstDesc.Format = DXGI_FORMAT_R32_FLOAT;
	dstDesc.Texture2D.MipLevels = 1;
	dstDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	dstDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_dev->CreateShaderResourceView(
		m_depthBuffer.Get(),
		&dstDesc,
		m_depthSRVHeap->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}

void DirectXCommon::ClearDepth()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();		
	// 深度バッファのクリア
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}