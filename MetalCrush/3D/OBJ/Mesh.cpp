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

	// �}�e���A���ÓI������
	Material::StaticInitialize(dev);
}

void Mesh::CreateBuffers()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * m_vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * m_indices.size());

	// ���_�o�b�t�@����
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

	// �C���f�b�N�X�o�b�t�@����
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

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(m_vertices.begin(), m_vertices.end(), vertMap);
		m_vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	uint32_t* indexMap = nullptr;
	result = m_indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(m_indices.begin(), m_indices.end(), indexMap);
		m_indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	m_vbView.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = sizeVB;
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	m_ibView.BufferLocation = m_indexBuff->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R32_UINT;
	m_ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&m_ibView);

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(3, m_material->GetGPUHandle());
	// �}�e���A���̒萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(1, m_material->GetConstantBuffer()->GetGPUVirtualAddress());

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)m_indices.size(), 1, 0, 0, 0);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	auto itr = m_smoothData.begin();
	for (; itr != m_smoothData.end(); ++itr)
	{
		//�e�ʂ̋��ʒ��_�R���N�V����
		std::vector<uint32_t>& v = itr->second;
		//�S���_�̖@���𕽋ω�����
		XMVECTOR normal = {};
		for (uint32_t index : v)
		{
			normal += XMVectorSet(m_vertices[index].normal.x, m_vertices[index].normal.y, m_vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//���ʖ@�����g�p���邷�ׂĂ̒��_�f�[�^�ɏ�������
		for (uint32_t index : v)
		{
			m_vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}
