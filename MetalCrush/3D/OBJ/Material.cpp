#include "Material.h"
#include <DirectXTex.h>
#include <cassert>

using namespace DirectX;
using namespace std;

ID3D12Device* Material::s_dev = nullptr;

Material::Material()
{
	ambient = { 1.0f, 1.0f, 1.0f };
	diffuse = { 0.8f, 0.8f, 0.8f };
	specular = { 0.5f, 0.5f, 0.5f };
	alpha = 1.0f;
}

void Material::StaticInitialize(ID3D12Device* dev)
{
	assert(dev);

	Material::s_dev = dev;
}

Material* Material::Create()
{
	Material* instance = new Material;

	instance->Initialize();

	return instance;
}

void Material::Initialize()
{
	CreateConstBuffer();
}

void Material::Update()
{
	HRESULT result;

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		m_constBuff->Unmap(0, nullptr);
	}
}

void Material::CreateConstBuffer()
{
	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));
	if (FAILED(result))
	{
		assert(0);
	}
}

void Material::LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV)
{
	HRESULT result = S_FALSE;

	// �e�N�X�`���Ȃ�
	if (textureFilename.size() == 0)
	{
		textureFilename = "default/white1x1.png";
	}

	m_cpuDescHandleSRV = cpuDescHandleSRV;
	m_gpuDescHandleSRV = gpuDescHandleSRV;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//�t�@�C���p�X������
	string filepath = directoryPath + textureFilename;
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//�e�N�X�`���̃��[�h
	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result))
	{
		assert(0);
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o
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
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&m_texbuff));
	if (FAILED(result))
	{
		assert(0);
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = m_texbuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch); // 1���T�C�Y
	if (FAILED(result))
	{
		assert(0);
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = m_texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	s_dev->CreateShaderResourceView(m_texbuff.Get(), // �r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, // �e�N�X�`���ݒ���
		m_cpuDescHandleSRV
	);
}
