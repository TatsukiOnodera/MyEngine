﻿#include "ParticleManager.h"

#include <time.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float ParticleManager::c_radius = 5.0f;				// 底面の半径
const float ParticleManager::c_prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* ParticleManager::s_device = nullptr;
UINT ParticleManager::s_descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ParticleManager::s_cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::s_rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::s_pipelinestate;
ComPtr<ID3D12Resource> ParticleManager::s_vertBuff;
XMMATRIX ParticleManager::s_matView;
Camera *ParticleManager::s_camera = nullptr;
D3D12_VERTEX_BUFFER_VIEW ParticleManager::s_vbView{};
ParticleManager::VertexPos ParticleManager::s_vertices[c_vertexCount];
XMMATRIX ParticleManager::s_matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::s_matBillboardY = XMMatrixIdentity();

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

bool ParticleManager::StaticInitialize(ID3D12Device* device)
{
	// nullチェック
	assert(device);

	// デバイスとカメラ
	ParticleManager::s_device = device;
	ParticleManager::s_camera = Camera::GetInstance();

	// パイプライン初期化
	if (!InitializeGraphicsPipeline())
	{
		assert(0);
	}

	//頂点バッファの転送
	if (!InitializeVertBuffer())
	{
		assert(0);
	}

	return true;
}

ParticleManager* ParticleManager::Create(const std::string& textureName)
{
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager* particle = new ParticleManager();

	// 初期化
	particle->Initialize();

	//テクスチャ読み込み
	particle->LoadTexture(textureName);

	//戻り値
	return particle;
}

bool ParticleManager::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; // ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleVS.hlsl",	// シェーダファイル名
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

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
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
		L"Resources/Shaders/Particle/ParticlePS.hlsl",	// シェーダファイル名
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
		{ // xyz座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // カラー
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
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

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = s_device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&s_rootsignature));
	if (FAILED(result))
	{
		return result;
	}

	gpipeline.pRootSignature = s_rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = s_device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&s_pipelinestate));

	if (FAILED(result))
	{
		return result;
	}

	return true;
}

bool ParticleManager::LoadTexture(const std::string& textureName)
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = s_device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));//生成
	if (FAILED(result))
	{
		return false;
	}

	// デスクリプタサイズを取得
	s_descriptorHandleIncrementSize = s_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	std::string filePath = "Resources/" + textureName;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result))
	{
		return false;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = s_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&m_texbuff));
	if (FAILED(result))
	{
		return false;
	}

	// テクスチャバッファにデータ転送
	result = m_texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch); // 1枚サイズ
	if (FAILED(result))
	{
		return false;
	}

	// シェーダリソースビュー作成
	m_cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, s_descriptorHandleIncrementSize);
	m_gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, s_descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = m_texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	s_device->CreateShaderResourceView(m_texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		m_cpuDescHandleSRV
	);

	return true;
}

bool ParticleManager::InitializeVertBuffer()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = s_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(s_vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&s_vertBuff));
	if (FAILED(result))
	{
		return false;
	}

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = s_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, s_vertices, sizeof(s_vertices));
		s_vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	s_vbView.BufferLocation = s_vertBuff->GetGPUVirtualAddress();
	s_vbView.SizeInBytes = sizeof(s_vertices);
	s_vbView.StrideInBytes = sizeof(s_vertices[0]);

	return true;
}

void ParticleManager::UpdateViewMatrix()
{
	//視点
	XMVECTOR eyePosition = XMLoadFloat3(&s_camera->GetEye());
	//注視点
	XMVECTOR targetPosition = XMLoadFloat3(&s_camera->GetTarget());
	//上方向
	XMVECTOR upVector = XMLoadFloat3(&s_camera->GetUp());

	//カメラZ軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//カメラのX軸
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求める
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求める
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//ベクトルを正規化
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//転置により逆行列を計算
	s_matView = XMMatrixTranspose(matCameraRot);

	//視点座標に-1をかけた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル
	XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition);
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー行列に平行移動成分を設定
	s_matView.r[3] = translation;

	//ビルボード行列
	s_matBillboard.r[0] = cameraAxisX;
	s_matBillboard.r[1] = cameraAxisY;
	s_matBillboard.r[2] = cameraAxisZ;
	s_matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	//カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求める
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y軸回りビルボード行列
	s_matBillboardY.r[0] = ybillCameraAxisX;
	s_matBillboardY.r[1] = ybillCameraAxisY;
	s_matBillboardY.r[2] = ybillCameraAxisZ;
	s_matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
}

void ParticleManager::Initialize()
{
	// nullptrチェック
	assert(s_device);

	HRESULT result;

	// 定数バッファの生成
	result = s_device->CreateCommittedResource(
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
}

void ParticleManager::Update()
{
	HRESULT result;

	// ビュー行列
	UpdateViewMatrix();

	//寿命が尽きたパーティクルを全削除
	m_partices.remove_if(
		[](Particle& x) {
			return x.frame >= x.endFrame;
		}
	);

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator it = m_partices.begin(); it != m_partices.end(); it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		// 速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		// 速度による移動
		if (it->isFollow == true && it->targetPosition != nullptr)
		{
			it->position = it->position + it->velocity;
			XMFLOAT3 tmp = {};
			tmp.x = it->targetPosition->x;
			tmp.y = it->targetPosition->y;
			tmp.z = it->targetPosition->z;
			it->worldPosition = tmp + it->position;
		}
		else
		{
			it->worldPosition = it->worldPosition + it->velocity;
		}

		float f = (float)it->endFrame / it->frame;

		// RGBA
		it->color = it->endColor - it->startColor;
		it->color.x /= f;
		it->color.y /= f;
		it->color.z /= f;
		it->color = it->color + it->startColor;

		//	スケール
		it->scale = (it->endScale - it->startScale) / f;
		it->scale += it->startScale;
	}

	//頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = s_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<Particle>::iterator it = m_partices.begin(); it != m_partices.end(); it++)
		{
			vertMap->pos = it->worldPosition;
			vertMap->scale = it->scale;
			vertMap->color = it->color;
			vertMap++;
		}
		s_vertBuff->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = s_matView * s_camera->GetMatProject();
	constMap->matBillboard = s_matBillboard;
	m_constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (!m_partices.empty())
	{
		//更新
		Update();

		// nullチェック
		assert(cmdList);

		// コマンドリストをセット
		ParticleManager::s_cmdList = cmdList;

		// パイプラインステートの設定
		cmdList->SetPipelineState(s_pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(s_rootsignature.Get());
		// プリミティブ形状を設定
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		// 頂点バッファの設定
		cmdList->IASetVertexBuffers(0, 1, &s_vbView);

		// デスクリプタヒープの配列
		ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// 定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(1, m_gpuDescHandleSRV);
		// 描画コマンド
		cmdList->DrawInstanced((UINT)std::distance(m_partices.begin(), m_partices.end()), 1, 0, 0);

		//コマンドリストを開放
		ParticleManager::s_cmdList = nullptr;
	}
}

void ParticleManager::Add(const int life, const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel, const XMFLOAT4& startColor, const XMFLOAT4& endColor, const float startScale, const float endScale, const bool isFollow, XMFLOAT3* targetPosition)
{
	//リストに要素を追加
	m_partices.emplace_front();
	//追加した要素の参照
	Particle& p = m_partices.front();
	//値のセット
	p.endFrame = life;
	p.position = { 0, 0, 0 };
	p.worldPosition = position;
	p.velocity = velocity;
	p.accel = accel;
	p.startColor = startColor;
	p.endColor = endColor;
	p.startScale = startScale;
	p.endScale = endScale;
	p.isFollow = isFollow;
	p.targetPosition = targetPosition;
}