#include "FbxModel.h"
#include <SafeDelete.h>

using namespace DirectX;

FbxModel::~FbxModel()
{
	//FBXシーンの開放
	fbxScene->Destroy();
}

void FbxModel::CreateBuffers(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;

	//メッシュのバッファ生成
	for (auto &m : meshes)
	{
		m->CreateBuffers(dev);
	}

	//テクスチャ画像データ
	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0); //生データ抽出
	assert(img);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texBuff));
	if (FAILED(result))
	{
		assert(0);
	}

	// テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch); // 1枚サイズ
	if (FAILED(result))
	{
		assert(0);
	}

	// SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビューの数
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));//生成
	if (FAILED(result))
	{
		assert(0);
	}

	// シェーダリソースビュー（SRV）作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()); //ヒープの先頭アドレス
}

void FbxModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//デストラクタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//メッシュの描画処理
	for (auto &m : meshes)
	{
		m->Draw(cmdList);
	}
}

const XMMATRIX& FbxModel::GetInverseGlobalTransform()
{
	return DirectX::XMMatrixInverse(nullptr, GetModelTransform());
}

void FbxModel::AddModelMesh(std::vector<FbxModelMesh*> mesh)
{
	meshes.clear();
	std::copy(mesh.begin(), mesh.end(), back_inserter(meshes));
}
