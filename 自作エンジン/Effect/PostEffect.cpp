#include "PostEffect.h"
#include "WinApp.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() : Sprite({ 500.0f, 500.0f }, 100, { 0.0f, 0.0f })
{

}

void PostEffect::Initialize()
{
	HRESULT result;

	//基底クラスとしての初期化
	Sprite::Initialize();

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WinApp::window_width,
		WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//テクスチャを赤クリア
	{
		//画素数
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		//画素数一行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		//画像全体のデータサイズ
		const UINT depthPitch = rowPitch * WinApp::window_height;
		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//デスクリプタヒープを設定
	D3D12_DESCRIPTOR_HEAP_DESC srcDescHeapDesc{};
	srcDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srcDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srcDescHeapDesc.NumDescriptors = 1;
	//デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&srcDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //２Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにシェーダリソースビュー作成
	dev->CreateShaderResourceView(texBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//非表示フラグがtrueなら
	if (isInvisible)
	{
		//描画せず抜ける
		return;
	}

	PreDraw(cmdList);

	//デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//頂点バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//シェーダリソースビューをセット
	/*cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			descriptorHandleIncrementSize));*/
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//ポリゴンの描画（４頂点で四角形）
	cmdList->DrawInstanced(vertNum, 1, 0, 0);

	PostDraw();
}
