#include "FbxModel.h"

using namespace DirectX;

FbxModel::~FbxModel()
{
	//FBXシーンの開放
	fbxScene->Destroy();
}

void FbxModel::CreateBuffers(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;

	for (int i = 0; i < meshes.size(); i++)
	{
		//頂点データ全体のサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * meshes[i].vertices.size());

		// 頂点バッファ生成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&meshes[i].vertBuff));

		// 頂点バッファへのデータ転送
		VertexPosNormalUvSkin* vertMap = nullptr;
		result = meshes[i].vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result))
		{
			std::copy(meshes[i].vertices.begin(), meshes[i].vertices.end(), vertMap);
			meshes[i].vertBuff->Unmap(0, nullptr);
		}

		// 頂点バッファビューの作成
		meshes[i].vbView.BufferLocation = meshes[i].vertBuff->GetGPUVirtualAddress();
		meshes[i].vbView.SizeInBytes = sizeVB;
		meshes[i].vbView.StrideInBytes = sizeof(meshes[i].vertices[0]);

		//インデックスバッファ全体のサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * meshes[i].indices.size());

		// インデックスバッファ生成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&meshes[i].indexBuff));
		if (FAILED(result))
		{
			assert(0);
			return;
		}

		// インデックスバッファへのデータ転送
		unsigned short* indexMap = nullptr;
		result = meshes[i].indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result))
		{
			std::copy(meshes[i].indices.begin(), meshes[i].indices.end(), indexMap);
			meshes[i].indexBuff->Unmap(0, nullptr);
		}

		// インデックスバッファビューの作成
		meshes[i].ibView.BufferLocation = meshes[i].indexBuff->GetGPUVirtualAddress();
		meshes[i].ibView.Format = DXGI_FORMAT_R16_UINT;
		meshes[i].ibView.SizeInBytes = sizeIB;
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

	for (int i = 0; i < meshes.size(); i++)
	{
		// 頂点バッファの設定
		cmdList->IASetVertexBuffers(0, 1, &meshes[i].vbView);
		// インデックスバッファの設定
		cmdList->IASetIndexBuffer(&meshes[i].ibView);

		// 描画コマンド
		cmdList->DrawIndexedInstanced((UINT)meshes[i].indices.size(), 1, 0, 0, 0);
	}
}

const XMMATRIX& FbxModel::GetInverseGlobalTransform()
{
	return DirectX::XMMatrixInverse(nullptr, GetModelTransform());
}

void FbxModel::CreateModelMesh()
{
	//mesh = new Mesh;
}

void FbxModel::SetModelMeshes()
{
	meshes.emplace_back(mesh);

	mesh.Delete();
}