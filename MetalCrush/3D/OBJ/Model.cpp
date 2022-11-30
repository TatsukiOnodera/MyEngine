#include "Model.h"

#include <DirectXMath.h>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const std::string Model::baseDirectory = "Resources/";
ID3D12Device* Model::s_dev = nullptr;
UINT Model::s_descriptorHandleIncrementSize = 0;

Model::Model()
{

}

Model::~Model()
{

}

void Model::StaticInitialize(ID3D12Device* dev)
{
	// nullptrチェック
	assert(dev);

	Model::s_dev = dev;

	// メッシュの静的初期化
	Mesh::StaticInitialize(dev);
}

Model *Model::Create(const std::string& modelName, const bool smooting)
{
	//インスタンス生成
	Model* model = new Model;

	//グラフィックスパイプライン生成
	model->m_graphicsPipeline.reset(new PipelineManager(s_dev));

	// 初期化
	model->Initialize(modelName, smooting);

	return model;
}

void Model::Initialize(const std::string& modelName, const bool smooting)
{
	HRESULT result = S_FALSE;

	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	const string filename = modelName + ".obj";
	const string directoryPath = baseDirectory + modelName + "/";
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	m_name = modelName;

	vector<XMFLOAT3> positions; //頂点座標
	vector<XMFLOAT3> normals; //法線ベクトル
	vector<XMFLOAT2> texcoords; //テクスチャUV
	// メッシュ生成
	Mesh* mesh = new Mesh;
	uint32_t indexCountTex = 0;
	uint32_t indexCountNoTex = 0;

	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭の文字がｖなら頂点座標
		if (key == "v")
		{
			//X, Y, Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			//U, V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn")
		{
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
			int faceIndexCount = 0;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				// マテリアル、テクスチャがある場合
				if (material && material->textureFilename.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
					index_stream >> indexNormal;
					// 頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);
				}
				else
				{
					char c;
					index_stream >> c;
					// スラッシュ2連続の場合、頂点番号のみ
					if (c == '/')
					{
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
					else
					{
						index_stream.seekg(-1, ios_base::cur); // 1文字戻る
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
						index_stream >> indexNormal;
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
				}
				//エッジ平滑化用のデータを追加
				if (smooting == true)
				{
					mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
				}
				//インデックスデータに追加
				if (faceIndexCount < 3)
				{
					mesh->AddIndex(indexCountTex);
				}
				else
				{
					mesh->AddIndex(indexCountTex - 3); //0
					mesh->AddIndex(indexCountTex - 1); //2
					mesh->AddIndex(indexCountTex); //3
				}
				//頂点カウント
				faceIndexCount++;
				//次のテクスチャの頂点追加
				indexCountTex++;
			}
		}
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr)
			{
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = m_materials.find(materialName);
				if (itr != m_materials.end())
				{
					mesh->SetMaterial(itr->second.get());
				}
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
		// 先頭文字列がgならグループの開始
		if (key == "g")
		{
			// カレントメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0)
			{
				// コンテナに登録
				m_meshes.emplace_back(mesh);
				// 次のメッシュ生成
				mesh = new Mesh;
				indexCountTex = 0;
			}
			// グループ名読み込み
			string groupName;
			line_stream >> groupName;
			// メッシュに名前をセット
			mesh->SetName(groupName);
		}
	}
	file.close();

	// コンテナに登録
	m_meshes.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : m_meshes)
	{
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr)
		{
			if (defaultMaterial == nullptr)
			{
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				m_materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	// メッシュのバッファ生成とスムージング
	for (auto& m : m_meshes)
	{
		m->CalculateSmoothedVertexNormals();
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : m_materials)
	{
		m.second->Update();
	}

	// テクスチャの読み込み
	LoadTextures();
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_descHeap)
	{
		// デスクリプタヒープの配列
		ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap.Get() };
		//デストラクタヒープをセット
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	for (auto& m : m_meshes)
	{
		m->Draw(cmdList);
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

	Material* material = nullptr;

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
			// 既にマテリアルがあれば
			if (material)
			{
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			line_stream >> material->name;
		}
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}
		if (key == "map_Kd")
		{
			//テクスチャファイル名読み込み
			line_stream >> material->textureFilename;
		}
	}

	file.close();

	if (material)
	{
		// マテリアル登録
		AddMaterial(material);
	}
}

void Model::LoadTextures()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	if (0 < m_materials.size())
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NumDescriptors = static_cast<UINT>(m_materials.size()); // シェーダーリソースビューの数
		result = s_dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));
		if (FAILED(result))
		{
			assert(0);
		}
	}
	// デスクリプタサイズを取得
	s_descriptorHandleIncrementSize = s_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 頂点インデックス
	int textureIndex = 0;
	// ディレクトリパス
	string directoryPath = baseDirectory + m_name + "/";

	for (auto& m : m_materials)
	{
		Material* material = m.second.get();

		// テクスチャあり
		if (material->textureFilename.size() > 0)
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);
		}
		else
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(baseDirectory, cpuDescHandleSRV, gpuDescHandleSRV);
		}
		textureIndex++;
	}
}
