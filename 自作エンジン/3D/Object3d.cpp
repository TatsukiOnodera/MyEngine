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
ID3D12Device* Object3d::dev = nullptr;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
Camera *Object3d::camera = nullptr;
Light* Object3d::light = nullptr;

Object3d::~Object3d()
{
	if (collider)
	{
		delete collider;
	}
}

bool Object3d::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	if (device == nullptr)
	{
		return false;
	}
	Object3d::dev = device;
	Object3d::camera = Camera::GetInstance();

	Model::StaticInitialize(dev);

	return true;
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	//nullチェック
	assert(commandList);
	Object3d::cmdList = commandList;

	//パイプラインとルートシグネチャの設定
	cmdList->SetPipelineState(graphicsPipeline->GetPipelineState());
	cmdList->SetGraphicsRootSignature(graphicsPipeline->GetRootSignature());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	cmdList = nullptr;
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
	graphicsPipeline.reset(new PipelineManager(dev));
	assert(graphicsPipeline->GetPipelineState());
	assert(graphicsPipeline->GetRootSignature());

	// 定数バッファB0の生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	name = typeid(*this).name();

	model->Initialize();
}

void Object3d::Update()
{
	if (dirty == true || camera->GetDirty() == true || light->GetDirty() == true)
	{
		//ワールド行列の更新
		if (isBillboard == true)
		{
			//ビルボード行列の更新
			matWorld = XMMatrixIdentity(); //単位行列
			//拡大行列
			matWorld *= XMMatrixScaling(scale.x, scale.y, scale.z);
			//回転行列
			matWorld *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
			matWorld *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
			matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
			//ビルボード行列
			matWorld *= camera->GetMatBillboard();
			//平行移動行列
			matWorld *= XMMatrixTranslation(position.x, position.y, position.z);
		} 
		else
		{
			matWorld = XMMatrixIdentity(); //単位行列
			//拡大行列
			matWorld *= XMMatrixScaling(scale.x, scale.y, scale.z);
			//回転行列
			matWorld *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
			matWorld *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
			matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
			//平行移動行列
			matWorld *= XMMatrixTranslation(position.x, position.y, position.z);
		}

		const XMMATRIX& matView = camera->GetMatView();
		const XMMATRIX& matProjection = camera->GetMatProject();

		///定数バッファ転送
		ConstBufferData* constMap = nullptr;
		HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matView * matProjection;
			constMap->world = matWorld;
			constMap->cameraPos = camera->GetEye();
			constMap->color = color;
			constBuff->Unmap(0, nullptr);
		}
		model->Update();

		if (collider)
		{
			collider->Update();
		}

		dirty = false;
	}
}

void Object3d::Draw(ID3D12GraphicsCommandList* commandList)
{
	//モデルがないなら抜ける
	if (model == nullptr)
	{
		return;
	}

	//更新
	Update();

	//描画前処理
	PreDraw(commandList);
	
	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//ライトの描画
	light->Draw(cmdList, 3);

	//モデル描画
	model->Draw(cmdList);

	//描画後処理
	PostDraw();
}

void Object3d::SetPosition(XMFLOAT3 position)
{
	this->position = position;
	dirty = true;
}

void Object3d::SetRotation(XMFLOAT3 rotation)
{
	this->rotation = rotation;
	dirty = true;
}

void Object3d::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
	dirty = true;
}

void Object3d::SetColor(XMFLOAT4 color)
{
	this->color = color;
	dirty = true;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);

	this->collider = collider;
}

void Object3d::SetGraphicsPipeline(const int shaderType)
{
	//nullチェック
	assert(graphicsPipeline);

	//指定のシェーダー生成
	switch (shaderType)
	{
	case ADS:
		graphicsPipeline->CreateADSPipeline(dev);
		break;

	case TOON:
		graphicsPipeline->CreateToonPipeline(dev);
		break;

	default:
		assert(0);
	}
}
