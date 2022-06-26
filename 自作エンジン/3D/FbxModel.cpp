#include "FbxModel.h"

using namespace DirectX;

FbxModel::~FbxModel()
{
	//FBX�V�[���̊J��
	fbxScene->Destroy();
}

void FbxModel::CreateBuffers(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;

	for (int i = 0; i < meshes.size(); i++)
	{
		//���_�f�[�^�S�̂̃T�C�Y
		UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * meshes[i].vertices.size());

		// ���_�o�b�t�@����
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&meshes[i].vertBuff));

		// ���_�o�b�t�@�ւ̃f�[�^�]��
		VertexPosNormalUvSkin* vertMap = nullptr;
		result = meshes[i].vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result))
		{
			std::copy(meshes[i].vertices.begin(), meshes[i].vertices.end(), vertMap);
			meshes[i].vertBuff->Unmap(0, nullptr);
		}

		// ���_�o�b�t�@�r���[�̍쐬
		meshes[i].vbView.BufferLocation = meshes[i].vertBuff->GetGPUVirtualAddress();
		meshes[i].vbView.SizeInBytes = sizeVB;
		meshes[i].vbView.StrideInBytes = sizeof(meshes[i].vertices[0]);

		//�C���f�b�N�X�o�b�t�@�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * meshes[i].indices.size());

		// �C���f�b�N�X�o�b�t�@����
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

		// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
		unsigned short* indexMap = nullptr;
		result = meshes[i].indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result))
		{
			std::copy(meshes[i].indices.begin(), meshes[i].indices.end(), indexMap);
			meshes[i].indexBuff->Unmap(0, nullptr);
		}

		// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
		meshes[i].ibView.BufferLocation = meshes[i].indexBuff->GetGPUVirtualAddress();
		meshes[i].ibView.Format = DXGI_FORMAT_R16_UINT;
		meshes[i].ibView.SizeInBytes = sizeIB;
	}

	//�e�N�X�`���摜�f�[�^
	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o
	assert(img);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuff));
	if (FAILED(result))
	{
		assert(0);
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch); // 1���T�C�Y
	if (FAILED(result))
	{
		assert(0);
	}

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[�̐�
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));//����
	if (FAILED(result))
	{
		assert(0);
	}

	// �V�F�[�_���\�[�X�r���[�iSRV�j�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texBuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()); //�q�[�v�̐擪�A�h���X
}

void FbxModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//�f�X�g���N�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	for (int i = 0; i < meshes.size(); i++)
	{
		// ���_�o�b�t�@�̐ݒ�
		cmdList->IASetVertexBuffers(0, 1, &meshes[i].vbView);
		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		cmdList->IASetIndexBuffer(&meshes[i].ibView);

		// �`��R�}���h
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