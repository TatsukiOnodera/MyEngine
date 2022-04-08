#include "Object3d.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "BaseCollider.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

// 静的メンバ変数の実体
ID3D12Device* Object3d::dev = nullptr;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3d::rootsignature;
ComPtr<ID3D12PipelineState> Object3d::pipelinestate;
Camera *Object3d::camera = nullptr;
Light* Object3d::light = nullptr;

Object3d::~Object3d()
{
	if (collider)
	{
		delete collider;
	}
}

bool Object3d::StaticInitialize(ID3D12Device* device, Camera* camera, int window_width, int window_height)
{
	if (device == nullptr || camera == nullptr)
	{
		return false;
	}

	Object3d::dev = device;
	Object3d::camera = camera;

	Model::StaticInitialize(device);

	CreateGraphicsPipeline();

	return true;
}

void Object3d::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob>psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob>errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/OBJVertexShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/OBJPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> rootSigBlob;

	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	if (FAILED(result))
	{
		assert(0);
	}

	//ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) 
	{
		assert(0);
	}

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();

	//パイプラインステートの生成
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	if (FAILED(result))
	{
		assert(0);
	}
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	Object3d::cmdList = cmdList;

	//パイプラインとルートシグネチャの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
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

	XMMATRIX matView = camera->GetMatView();
	XMMATRIX matProjection = camera->GetMatProject();

	///定数バッファ転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->viewproj = matView * matProjection;
	constMap->world = matWorld;
	constMap->cameraPos = camera->GetEye();
	constMap->color = color;
	constBuff->Unmap(0, nullptr);

	model->Update(model->GetMaterial());

	if (collider)
	{
		collider->Update();
	}
}

void Object3d::Draw()
{
	//更新
	Update();
	
	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//ライトの描画
	light->Draw(cmdList, 3);

	//モデル描画
	model->Draw(cmdList);
}

void Object3d::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}

void Object3d::SetRotation(XMFLOAT3 rotation)
{
	this->rotation = rotation;
}

void Object3d::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void Object3d::SetColor(XMFLOAT4 color)
{
	this->color = color;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);

	this->collider = collider;
}
