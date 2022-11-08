#include "LightGroup.h"

using namespace DirectX;

ID3D12Device* LightGroupdev = nullptr;

bool LightGroupStaticInitialize(ID3D12Device* dev)
{
	if (dev == nullptr)
	{
		return false;
	}

	LightGroupdev = dev;

	return true;
}

Light* LightGroupCreate()
{
	Light* instance = new Light();

	instance->Initialize();

	instance->Update();

	return instance;
}

void LightGroupInitialize()
{
	HRESULT result;
	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result))
	{
		assert(0);
	}

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroupTransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightDir;	// 行列の合成
		constMap->lightcolor = lightColor;
		constBuff->Unmap(0, nullptr);
	}
}

void LightGroupSetLightDir(const XMVECTOR& lightDir)
{
	this->lightDir = XMVector3Normalize(lightDir);

	dirty = true;
}

void LightGroupSetLightColor(const XMFLOAT3& lightColor)
{
	this->lightColor = lightColor;

	dirty = true;
}

void LightGroupUpdate()
{
	if (dirty == true)
	{
		TransferConstBuffer();
		dirty = false;
		isDirty = true;
	} else
	{
		isDirty = false;
	}
}

void LightGroupDraw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}