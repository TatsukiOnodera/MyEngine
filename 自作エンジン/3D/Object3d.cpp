#include "Object3d.h"
#include "BaseCollider.h"

#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

// 静的メンバ変数の実体
ID3D12Device* Object3d::s_dev = nullptr;
ID3D12GraphicsCommandList* Object3d::s_cmdList = nullptr;
Camera *Object3d::s_camera = nullptr;
Light* Object3d::s_light = nullptr;

Object3d::~Object3d()
{

}

bool Object3d::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	if (device == nullptr)
	{
		return false;
	}
	Object3d::s_dev = device;
	Object3d::s_camera = Camera::GetInstance();

	Model::StaticInitialize(s_dev);

	return true;
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//nullチェック
	assert(cmdList);
	Object3d::s_cmdList = cmdList;

	//パイプラインとルートシグネチャの設定
	s_cmdList->SetPipelineState(m_graphicsPipeline->GetPipelineState());
	s_cmdList->SetGraphicsRootSignature(m_graphicsPipeline->GetRootSignature());
	//プリミティブ形状を設定
	s_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	s_cmdList = nullptr;
}

Object3d* Object3d::Create(const std::string& modelName, bool smooting)
{
	Object3d* object = new Object3d;

	Model* model = Model::CreateModel(modelName, smooting);

	object->SetModel(model);

	object->Initialize();

	object->Update();

	return object;
}

void Object3d::Initialize()
{
	HRESULT result;

	//パイプライン生成
	m_graphicsPipeline.reset(new PipelineManager(s_dev));
	assert(m_graphicsPipeline->GetPipelineState());
	assert(m_graphicsPipeline->GetRootSignature());

	// 定数バッファB0の生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));

	m_name = typeid(*this).name();

	m_model->Initialize();
}

void Object3d::Update()
{
	if (m_dirty || s_camera->GetDirty() || s_light->GetDirty())
	{
		//ワールド行列の更新
		if (m_isBillboard)
		{
			//ビルボード行列の更新
			m_matWorld = XMMatrixIdentity(); //単位行列
			//拡大行列
			m_matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
			//回転行列
			m_matWorld *= XMMatrixRotationX(XMConvertToRadians(m_rotation.x));
			m_matWorld *= XMMatrixRotationY(XMConvertToRadians(m_rotation.y));
			m_matWorld *= XMMatrixRotationZ(XMConvertToRadians(m_rotation.z));
			//ビルボード行列
			m_matWorld *= s_camera->GetMatBillboard();
			//平行移動行列
			m_matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		} 
		else
		{
			m_matWorld = XMMatrixIdentity(); //単位行列
			//拡大行列
			m_matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
			//回転行列
			m_matWorld *= XMMatrixRotationX(XMConvertToRadians(m_rotation.x));
			m_matWorld *= XMMatrixRotationY(XMConvertToRadians(m_rotation.y));
			m_matWorld *= XMMatrixRotationZ(XMConvertToRadians(m_rotation.z));
			//平行移動行列
			m_matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}

		const XMMATRIX& matView = s_camera->GetMatView();
		const XMMATRIX& matProjection = s_camera->GetMatProject();

		///定数バッファ転送
		ConstBufferData* constMap = nullptr;
		HRESULT result = m_constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matView * matProjection;
			constMap->world = m_matWorld;
			constMap->cameraPos = s_camera->GetEye();
			constMap->color = m_color;
			m_constBuff->Unmap(0, nullptr);
		}

		//モデルデータ更新
		m_model->Update();

		if (m_collider)
		{
			m_collider->Update();
		}

		m_dirty = false;
	}
}

void Object3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//モデルがないなら抜ける
	if (m_model == nullptr)
	{
		return;
	}

	//更新
	Update();

	//描画前処理
	PreDraw(cmdList);
	
	//定数バッファをセット
	s_cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());

	//ライトの描画
	s_light->Draw(s_cmdList, 3);

	//モデル描画
	m_model->Draw(s_cmdList);

	//描画後処理
	PostDraw();
}

void Object3d::SetPosition(XMFLOAT3 position)
{
	this->m_position = position;
	m_dirty = true;
}

void Object3d::SetRotation(XMFLOAT3 rotation)
{
	this->m_rotation = rotation;
	m_dirty = true;
}

void Object3d::SetScale(XMFLOAT3 scale)
{
	this->m_scale = scale;
	m_dirty = true;
}

void Object3d::SetColor(XMFLOAT4 color)
{
	this->m_color = color;
	m_dirty = true;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);

	this->m_collider.reset(collider);
}

void Object3d::SetGraphicsPipeline(const int shaderType)
{
	//nullチェック
	assert(m_graphicsPipeline);

	//指定のシェーダー生成
	switch (shaderType)
	{
	case ADS:
		m_graphicsPipeline->CreateADSPipeline(s_dev);
		break;

	case TOON:
		m_graphicsPipeline->CreateToonPipeline(s_dev);
		break;

	default:
		assert(0);
	}
}
