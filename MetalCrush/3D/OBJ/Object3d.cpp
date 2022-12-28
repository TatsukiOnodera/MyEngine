#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

#include <SafeDelete.h>
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
LightGroup* Object3d::s_lightGroup = nullptr;
std::unique_ptr<Model> Object3d::s_modelList[c_modelMaxCount];

Object3d::~Object3d()
{
	if (m_collider)
	{
		CollisionManager::GetInstance()->RemoveCollider(m_collider.get());
	}
}

bool Object3d::StaticInitialize(ID3D12Device* device)
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

	//プリミティブ形状を設定
	s_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	s_cmdList = nullptr;
}

Model* Object3d::GetModel(const UINT modelNumber)
{
	assert(modelNumber <= c_modelMaxCount - 1);

	// 既にあるなら
	if (!(s_modelList[modelNumber]))
	{
		assert(0);
	}

	return s_modelList[modelNumber].get();
}

void Object3d::LoadModel(const UINT modelNumber, const std::string& modelName, bool smoothing)
{
	assert(modelNumber <= c_modelMaxCount - 1);

	// 既にあるなら
	if (s_modelList[modelNumber])
	{
		return;
	}

	// モデル作成
	s_modelList[modelNumber].reset(Model::Create(modelName, smoothing));
}

Object3d* Object3d::Create(const UINT modelNumber)
{
	// モデルがあるか
	if (!(s_modelList[modelNumber]))
	{
		assert(0);
	}

	Object3d* object = new Object3d;

	object->SetModel(s_modelList[modelNumber].get());

	object->Initialize();

	object->Update();

	return object;
}

void Object3d::Initialize()
{
	HRESULT result;

	// 定数バッファB0の生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));
	
	// クラス名取得
	name = typeid(*this).name();
}

void Object3d::Update()
{
	if (m_dirty == true || s_camera->GetDirty() == true)
	{
		// 行列の更新
		UpdateWorldMatrix();

		// ビュー行列
		const XMMATRIX& matView = s_camera->GetMatView();
		// 射影行列
		const XMMATRIX& matProjection = s_camera->GetMatProject();

		///定数バッファ転送
		ConstBufferData* constMap = nullptr;
		HRESULT result = m_constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matView * matProjection;
			// 親がいるなら行列をかける
			if (m_parent)
			{
				m_matWorld = m_matWorld * m_parent->GetMatWorld();
			}
			constMap->world = m_matWorld;
			constMap->cameraPos = s_camera->GetEye();
			constMap->color = m_color;
			constMap->shininess = m_shininess;
			constMap->tiling = m_tiling;
			constMap->offset = m_offset;
			m_constBuff->Unmap(0, nullptr);
		}

		// コライダーの更新
		if (m_collider)
		{
			m_collider->Update();
		}

		m_dirty = false;
	}
}

void Object3d::UpdateWorldMatrix()
{
	// ワールド行列の更新
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
		m_matWorld *= m_isBillboard;
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
}

void Object3d::Draw()
{
	//モデルがないか画面外なら抜ける
	if (m_model == nullptr)
	{
		return;
	}

	//更新
	Update();
	
	//NULLチェック
	assert(s_cmdList);

	//パイプラインとルートシグネチャの設定
	s_cmdList->SetPipelineState(m_model->GetPipelineState());
	s_cmdList->SetGraphicsRootSignature(m_model->GetRootSignature());
	
	//定数バッファをセット
	s_cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());

	//ライトの描画
	s_lightGroup->Draw(s_cmdList, 2);

	//モデル描画
	m_model->Draw(s_cmdList);
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->m_collider.reset(collider);

	CollisionManager::GetInstance()->AddCollider(collider);
	m_collider->Update();
}

void Object3d::OnCollision(const CollisionInfo& info)
{

}
