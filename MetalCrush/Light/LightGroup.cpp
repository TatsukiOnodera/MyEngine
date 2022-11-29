#include "LightGroup.h"

using namespace DirectX;

ID3D12Device* LightGroup::s_dev = nullptr;

bool LightGroup::StaticInitialize(ID3D12Device* dev)
{
	if (dev == nullptr)
	{
		return false;
	}

	LightGroup::s_dev = dev;

	return true;
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup;

	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	// 初期設定
	DefaultLightSetting();

	HRESULT result;
	// 定数バッファの生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));
	if (FAILED(result))
	{
		assert(0);
	}

	// 定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		// 環境光
		constMap->ambientColor = m_ambientColor;
		// 平行光源
		for (int i = 0; i < c_dirLightNum; i++)
		{
			if (m_dirLights[i].GetActive() == true)
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightDir = -m_dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = m_dirLights[i].GetLightColor();
			}
			else
			{
				constMap->dirLights[i].active = 0;
			}
		}
		// 点光源
		for (int i = 0; i < c_pointLightNum; i++)
		{
			if (m_pointLights[i].GetActive() == true)
			{
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightPos = m_pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = m_pointLights[i].GetLightColor();
				constMap->pointLights[i].lightAtten = m_pointLights[i].GetLightAtten();
			}
			else
			{
				constMap->pointLights[i].active = 0;
			}
		}
		// スポットライト
		for (int i = 0; i < c_spotLightNum; i++)
		{
			if (m_spotLights[i].GetActive() == true)
			{
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightDir = -m_spotLights[i].GetLightDir();
				constMap->spotLights[i].lightPos = m_spotLights[i].GetLightPos();
				constMap->spotLights[i].lightColor = m_spotLights[i].GetLightColor();
				constMap->spotLights[i].lightAtten = m_spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightFactorAngleCos = m_spotLights[i].GetLightFactorAngleCos();
			}
			else
			{
				constMap->spotLights[i].active = 0;
			}
		}
		// 丸影
		for (int i = 0; i < c_circleShadowNum; i++)
		{
			if (m_circleShadows[i].GetActive() == true)
			{
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -m_circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos = m_circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = m_circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = m_circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = m_circleShadows[i].GetFactorAngleCos();
			}
			else
			{
				constMap->circleShadows[i].active = 0;
			}
		}
		m_constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	m_dirLights[0].SetActive(false);
}

void LightGroup::Update()
{
	if (m_dirty == true)
	{
		TransferConstBuffer();

		m_dirty = false;
		m_isDirty = true;
	}
	else
	{
		m_isDirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_constBuff->GetGPUVirtualAddress());
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	m_ambientColor = color;

	m_dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < c_dirLightNum);

	m_dirLights[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightDir)
{
	assert(0 <= index && index < c_dirLightNum);

	m_dirLights[index].SetLightDir(lightDir);
	m_dirty = true;
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < c_dirLightNum);

	m_dirLights[index].SetLightColor(lightColor);
	m_dirty = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < c_pointLightNum);

	m_pointLights[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetPointLightPos(int index, const XMFLOAT3& lightPos)
{
	assert(0 <= index && index < c_pointLightNum);

	m_pointLights[index].SetLightPos(lightPos);
	m_dirty = true;
}

void LightGroup::SetPointLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < c_pointLightNum);

	m_pointLights[index].SetLightColor(lightColor);
	m_dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < c_pointLightNum);

	m_pointLights[index].SetLightAtten(lightAtten);
	m_dirty = true;
}

void LightGroup::SetSpotLightDir(int index, const XMVECTOR& lightDir)
{
	assert(0 <= index && index < c_spotLightNum);

	m_spotLights[index].SetLightDir(lightDir);
	m_dirty = true;
}

void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < c_spotLightNum);

	m_spotLights[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetSpotLightPos(int index, const XMFLOAT3& lightPos)
{
	assert(0 <= index && index < c_spotLightNum);

	m_spotLights[index].SetLightPos(lightPos);
	m_dirty = true;
}

void LightGroup::SetSpotLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < c_spotLightNum);

	m_spotLights[index].SetLightColor(lightColor);
	m_dirty = true;
}

void LightGroup::SetSpotLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < c_spotLightNum);

	m_spotLights[index].SetLightAtten(lightAtten);
	m_dirty = true;
}

void LightGroup::SetSpotLightFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos)
{
	assert(0 <= index && index < c_spotLightNum);

	m_spotLights[index].SetLightFactorAngleCos(lightFactorAngleCos);
	m_dirty = true;
}

void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < c_circleShadowNum);

	m_circleShadows[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos)
{
	assert(0 <= index && index < c_circleShadowNum);

	m_circleShadows[index].SetCasterPos(casterPos);
	m_dirty = true;
}

void LightGroup::SetCircleShadowDir(int index, const XMVECTOR& lightDir)
{
	assert(0 <= index && index < c_circleShadowNum);

	m_circleShadows[index].SetDir(lightDir);
	m_dirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < c_circleShadowNum);

	m_circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
	m_dirty = true;
}

void LightGroup::SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < c_circleShadowNum);

	m_circleShadows[index].SetAtten(lightAtten);
	m_dirty = true;
}

void LightGroup::SetCircleShadowFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos)
{
	assert(0 <= index && index < c_circleShadowNum);

	m_circleShadows[index].SetFactorAngleCos(lightFactorAngleCos);
	m_dirty = true;
}
