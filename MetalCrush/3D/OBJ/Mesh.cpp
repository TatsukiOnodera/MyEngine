#include "Mesh.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* Mesh::s_dev = nullptr;

void Mesh::StaticInitialize(ID3D12Device* dev)
{
	assert(dev);

	Mesh::s_dev = dev;

	// マテリアル静的初期化
	Material::StaticInitialize(dev);
}

void Mesh::CreateBuffers()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * m_vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * m_indices.size());

	// 頂点バッファ生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertBuff));
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuff));
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(m_vertices.begin(), m_vertices.end(), vertMap);
		m_vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	uint32_t* indexMap = nullptr;
	result = m_indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(m_indices.begin(), m_indices.end(), indexMap);
		m_indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	m_vbView.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = sizeVB;
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);

	// インデックスバッファビューの作成
	m_ibView.BufferLocation = m_indexBuff->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R32_UINT;
	m_ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&m_ibView);

	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(3, m_material->GetGPUHandle());
	// マテリアルの定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(1, m_material->GetConstantBuffer()->GetGPUVirtualAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)m_indices.size(), 1, 0, 0, 0);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	auto itr = m_smoothData.begin();
	for (; itr != m_smoothData.end(); ++itr)
	{
		//各面の共通頂点コレクション
		std::vector<uint32_t>& v = itr->second;
		//全頂点の法線を平均化する
		XMVECTOR normal = {};
		for (uint32_t index : v)
		{
			normal += XMVectorSet(m_vertices[index].normal.x, m_vertices[index].normal.y, m_vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用するすべての頂点データに書き込む
		for (uint32_t index : v)
		{
			m_vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}
