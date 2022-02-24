#pragma once
#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <vector>
#include <string>
#include "WinApp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXCommon
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<IDXGIAdapter1> tmpAdapter;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	ComPtr<ID3D12Fence> fence;
	ComPtr<ID3D12Resource> depthBuffer;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	UINT64 fenceVal = 0;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	WinApp* win;

public:
	DirectXCommon();
	~DirectXCommon();

	ID3D12Device *GetDev() { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

	bool Initialize(WinApp* app);

	void OnDebugLayer();
	void PreDraw();
	void PostDraw();
	bool CreateDevice();
	bool CreateCmdList();
	bool CreateDoubleBuffer();
	bool CreateFence();
	bool CreateDepthBuffer();
	void ClearDepth();
};