#include "Light.h"

using namespace DirectX;

ID3D12Device* Light::dev = nullptr;

bool Light::StaticInitialize(ID3D12Device *dev)
{
	if (dev == nullptr)
	{
		return false;
	}

	Light::dev = dev;

	return true;
}

Light* Light::Create()
{
	Light* instance = new Light();

	instance->Initialize();

	instance->Update();

	return instance;
}

void Light::Initialize()
{
	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result))
	{
		assert(0);
	}

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void Light::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightDir;	// �s��̍���
		constMap->lightcolor = lightColor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const XMVECTOR& lightDir)
{
	this->lightDir = XMVector3Normalize(lightDir);

	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3& lightColor)
{
	this->lightColor = lightColor;

	dirty = true;
}

void Light::Update()
{
	if (dirty == true)
	{
		TransferConstBuffer();
		dirty = false;
		isDirty = true;
	}
	else
	{
		isDirty = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}