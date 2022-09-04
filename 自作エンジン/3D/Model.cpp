#include "Model.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream> //ファイル入出力
#include <sstream> //文字列に対する入出力
#include <string> //文字列
#include <vector> //頂点データをまとめるのに使う
#include <map>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Model::s_dev = nullptr;
UINT Model::s_descriptorHandleIncrementSize = 0;

void Model::StaticInitialize(ID3D12Device* dev)
{
	// nullptrチェック
	assert(dev);

	Model::s_dev = dev;
}

Model *Model::CreateModel(const std::string& modelName, bool smooting)
{
	//インスタンス生成
	Model* model = new Model;

	//グラフィックスパイプライン生成
	model->m_graphicsPipeline.reset(new PipelineManager(s_dev));

	//モデルデータ読み込み
	model->InitializeModel(modelName, smooting);

	//テクスチャの数が1より多いなら
	if (model->m_graphicsPipeline->GetTexNum() == 2)
	{
		//マスクテクスチャ名取得
		model->LoadTextureName("Resources/Default/", "red1x1.png");
	}
	else if (model->m_graphicsPipeline->GetTexNum() == 3)
	{
		//サブテクスチャ名取得
		model->LoadTextureName("Resources/Default/", "white1x1.png");
		//マスクテクスチャ名取得
		model->LoadTextureName("Resources/Default/", "red1x1.png");
	}

	//テクスチャの読み込み
	model->LoadTexture();

	return model;
}

void Model::Initialize()
{
	HRESULT result;

	// 定数バッファの生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));
}

void Model::Update()
{
	HRESULT result;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->ambient = m_material.ambient;
		constMap->diffuse = m_material.diffuse;
		constMap->specular = m_material.specular;
		constMap->alpha = m_material.alpha;
		m_constBuff->Unmap(0, nullptr);
	}
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&m_ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap.Get() };
	//デストラクタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(1, m_constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	for (int i = 0; i < m_graphicsPipeline->GetTexNum(); i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(3 + i, m_gpuDescHandleSRV[i]);
	}

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)m_indices.size(), 1, 0, 0, 0);
}

void Model::SetGraphicsPipeline(const int shaderType)
{
	//nullチェック
	assert(m_graphicsPipeline);

	//指定のシェーダー生成
	switch (shaderType)
	{
	case 0:
		m_graphicsPipeline->CreateADSPipeline(s_dev);
		break;

	case 1:
		m_graphicsPipeline->CreateToonPipeline(s_dev);
		break;

	case 2:
		m_graphicsPipeline->CreateMonochromaticPipeline(s_dev);
		break;

	case 3:
		m_graphicsPipeline->CreateTextureBlendPipeline(s_dev);
		break;

	case 4:
		m_graphicsPipeline->CreateSpecularMapPipeline(s_dev);
		break;

	default:
		assert(0);
	}

	textureName.resize(1);

	//テクスチャの数が1より多いなら
	if (m_graphicsPipeline->GetTexNum() == 2)
	{
		//マスクテクスチャ名取得
		LoadTextureName("Resources/Default/", "red1x1.png");
	}
	else if (m_graphicsPipeline->GetTexNum() == 3)
	{
		//サブテクスチャ名取得
		LoadTextureName("Resources/Default/", "black1x1.png");
		//マスクテクスチャ名取得
		LoadTextureName("Resources/Default/", "red1x1.png");
	}

	//テクスチャの読み込み
	LoadTexture();
}

void Model::SetSubTexture(const std::string& filename)
{
	if (!(m_graphicsPipeline->GetTexNum() == 3) || !(textureName.size() == 3))
	{
		return;
	}

	//サブテクスチャ名取得
	textureName[1] = filename;

	//テクスチャの読み込み
	LoadTexture();
}

void Model::SetMaskTexture(const std::string& filename)
{
	if (textureName.size() == 2 || textureName.size() == 3)
	{
		if (m_graphicsPipeline->GetTexNum() == 2)
		{
			//マスクテクスチャ名取得
			textureName[1] = filename;
		} 
		else if (m_graphicsPipeline->GetTexNum() == 3)
		{
			//マスクテクスチャ名取得
			textureName[2] = filename;
		}
	}
	else
	{
		return;
	}

	//テクスチャの読み込み
	LoadTexture();
}

void Model::InitializeModel(const std::string& modelName, bool smooting)
{
	HRESULT result = S_FALSE;

	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	const string filename = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	vector<XMFLOAT3> positions; //頂点座標
	vector<XMFLOAT3> normals; //法線ベクトル
	vector<XMFLOAT2> texcoords; //テクスチャUV
	unsigned short texVertex = 0; //テクスチャの要素数

	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭の文字がｖなら頂点座標
		if (key == "v") {
			//X, Y, Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}
		if (key == "vt") {
			//U, V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn") {
			//X, Y, Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			//頂点をカウントする
			int v_num = 0;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexNormal;
				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				m_vertices.emplace_back(vertex);
				//エッジ平滑化用のデータを追加
				if (smooting == true)
				{
					AddSmoothData(indexPosition, (unsigned short)m_vertices.size() - 1);
				}
				//インデックスデータに追加
				if (v_num < 3)
				{
					m_indices.emplace_back(texVertex);
				}
				else
				{
					m_indices.emplace_back(texVertex - 3); //0
					m_indices.emplace_back(texVertex - 1); //2
					m_indices.emplace_back(texVertex); //3
				}
				//頂点カウント
				v_num++;
				//次のテクスチャの頂点追加
				texVertex++;
			}
		}
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアルの読み込み
			LoadMaterial(directoryPath, filename);
		}
	}
	file.close();

	if (smooting == true)
	{
		CalculateSmoothedVertexNormals();
	}

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * m_vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * m_indices.size());

	// 頂点バッファ生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(m_vertices.begin(), m_vertices.end(), vertMap);
		m_vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = m_indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(m_indices.begin(), m_indices.end(), indexMap);
		m_indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	m_vbView.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = sizeVB;
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);

	// インデックスバッファビューの作成
	m_ibView.BufferLocation = m_indexBuff->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R16_UINT;
	m_ibView.SizeInBytes = sizeIB;
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	m_smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = m_smoothData.begin();
	for (; itr != m_smoothData.end(); ++itr)
	{
		//各面の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//全頂点の法線を平均化する
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(m_vertices[index].normal.x, m_vertices[index].normal.y, m_vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用するすべての頂点データに書き込む
		for (unsigned short index : v)
		{
			m_vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Model::LoadTexture()
{
	assert(0 < m_graphicsPipeline->GetTexNum() && m_graphicsPipeline->GetTexNum() <= 3);

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = m_graphicsPipeline->GetTexNum(); // シェーダーリソースビューの数
	result = s_dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	s_descriptorHandleIncrementSize = s_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < m_graphicsPipeline->GetTexNum(); i++)
	{
		// WICテクスチャのロード
		TexMetadata metadata{};
		ScratchImage scratchImg{};

		//ファイルパスを結合
		string filepath = textureName[i];

		//ユニコード文字列に変換する
		wchar_t wfilepath[128];
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

		//テクスチャのロード
		result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
		if (FAILED(result))
		{
			assert(0);
		}

		const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出
		assert(img);

		// リソース設定
		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels
		);

		// テクスチャ用バッファの生成
		result = s_dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
			nullptr,
			IID_PPV_ARGS(&m_texbuff[i]));
		if (FAILED(result)) 
		{
			assert(0);
		}

		// テクスチャバッファにデータ転送
		result = m_texbuff[i]->WriteToSubresource(
			0,
			nullptr, // 全領域へコピー
			img->pixels,    // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch); // 1枚サイズ
		if (FAILED(result)) 
		{
			assert(0);
		}

		// シェーダリソースビュー作成
		m_cpuDescHandleSRV[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), i, s_descriptorHandleIncrementSize);
		m_gpuDescHandleSRV[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), i, s_descriptorHandleIncrementSize);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
		D3D12_RESOURCE_DESC resDesc = m_texbuff[i]->GetDesc();

		srvDesc.Format = resDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;

		s_dev->CreateShaderResourceView(m_texbuff[i].Get(), //ビューと関連付けるバッファ
			&srvDesc, //テクスチャ設定情報
			m_cpuDescHandleSRV[i]);
	}
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename); //"Resources/triangle_mat/triangle_mat.obj"
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //先頭文字を削除
		}
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> m_material.name;
		}
		if (key == "Ka")
		{
			line_stream >> m_material.ambient.x;
			line_stream >> m_material.ambient.y;
			line_stream >> m_material.ambient.z;
		}
		if (key == "Kd") 
		{
			line_stream >> m_material.diffuse.x;
			line_stream >> m_material.diffuse.y;
			line_stream >> m_material.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> m_material.specular.x;
			line_stream >> m_material.specular.y;
			line_stream >> m_material.specular.z;
		}
		if (key == "map_Kd")
		{
			//テクスチャファイル名読み込み
			line_stream >> m_material.textureFilename;
			//テクスチャ読み込み
			LoadTextureName(directoryPath, m_material.textureFilename);
		}
	}
	//テクスチャを読み込んでいない
	if (m_material.textureFilename == "defaultTexture")
	{
		//テクスチャファイル名読み込み
		m_material.textureFilename = "white1x1.png";
		//テクスチャ読み込み
		LoadTextureName("Resources/Default/", m_material.textureFilename);
	}

	file.close();
}

void Model::LoadTextureName(const std::string& directoryPath, const std::string& filename)
{
	textureName.emplace_back(directoryPath + filename);
}
