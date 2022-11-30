#include "Model.h"

#include <DirectXMath.h>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
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
	// nullptr�`�F�b�N
	assert(dev);

	Model::s_dev = dev;

	// ���b�V���̐ÓI������
	Mesh::StaticInitialize(dev);
}

Model *Model::Create(const std::string& modelName, const bool smooting)
{
	//�C���X�^���X����
	Model* model = new Model;

	//�O���t�B�b�N�X�p�C�v���C������
	model->m_graphicsPipeline.reset(new PipelineManager(s_dev));

	// ������
	model->Initialize(modelName, smooting);

	return model;
}

void Model::Initialize(const std::string& modelName, const bool smooting)
{
	HRESULT result = S_FALSE;

	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	const string filename = modelName + ".obj";
	const string directoryPath = baseDirectory + modelName + "/";
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	m_name = modelName;

	vector<XMFLOAT3> positions; //���_���W
	vector<XMFLOAT3> normals; //�@���x�N�g��
	vector<XMFLOAT2> texcoords; //�e�N�X�`��UV
	// ���b�V������
	Mesh* mesh = new Mesh;
	uint32_t indexCountTex = 0;
	uint32_t indexCountNoTex = 0;

	//1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̕��������Ȃ璸�_���W
		if (key == "v")
		{
			//X, Y, Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			//U, V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn")
		{
			//X, Y, Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		if (key == "f")
		{
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			//���_���J�E���g����
			int faceIndexCount = 0;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
				// �}�e���A���A�e�N�X�`��������ꍇ
				if (material && material->textureFilename.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
					index_stream >> indexNormal;
					// ���_�f�[�^�̒ǉ�
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
					// �X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
					if (c == '/')
					{
						// ���_�f�[�^�̒ǉ�
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
					else
					{
						index_stream.seekg(-1, ios_base::cur); // 1�����߂�
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
						index_stream >> indexNormal;
						// ���_�f�[�^�̒ǉ�
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
				}
				//�G�b�W�������p�̃f�[�^��ǉ�
				if (smooting == true)
				{
					mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
				}
				//�C���f�b�N�X�f�[�^�ɒǉ�
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
				//���_�J�E���g
				faceIndexCount++;
				//���̃e�N�X�`���̒��_�ǉ�
				indexCountTex++;
			}
		}
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr)
			{
				// �}�e���A���̖��ǂݍ���
				string materialName;
				line_stream >> materialName;

				// �}�e���A�����Ō������A�}�e���A�������蓖�Ă�
				auto itr = m_materials.find(materialName);
				if (itr != m_materials.end())
				{
					mesh->SetMaterial(itr->second.get());
				}
			}
		}
		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			//�}�e���A���̓ǂݍ���
			LoadMaterial(directoryPath, filename);
		}
		// �擪������g�Ȃ�O���[�v�̊J�n
		if (key == "g")
		{
			// �J�����g���b�V���̏�񂪑����Ă���Ȃ�
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0)
			{
				// �R���e�i�ɓo�^
				m_meshes.emplace_back(mesh);
				// ���̃��b�V������
				mesh = new Mesh;
				indexCountTex = 0;
			}
			// �O���[�v���ǂݍ���
			string groupName;
			line_stream >> groupName;
			// ���b�V���ɖ��O���Z�b�g
			mesh->SetName(groupName);
		}
	}
	file.close();

	// �R���e�i�ɓo�^
	m_meshes.emplace_back(mesh);

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : m_meshes)
	{
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr)
		{
			if (defaultMaterial == nullptr)
			{
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				m_materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	// ���b�V���̃o�b�t�@�����ƃX���[�W���O
	for (auto& m : m_meshes)
	{
		m->CalculateSmoothedVertexNormals();
		m->CreateBuffers();
	}

	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : m_materials)
	{
		m.second->Update();
	}

	// �e�N�X�`���̓ǂݍ���
	LoadTextures();
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_descHeap)
	{
		// �f�X�N���v�^�q�[�v�̔z��
		ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap.Get() };
		//�f�X�g���N�^�q�[�v���Z�b�g
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	for (auto& m : m_meshes)
	{
		m->Draw(cmdList);
	}
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename); //"Resources/triangle_mat/triangle_mat.obj"
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	Material* material = nullptr;

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //�擪�������폜
		}
		if (key == "newmtl")
		{
			// ���Ƀ}�e���A���������
			if (material)
			{
				// �}�e���A�����R���e�i�ɓo�^
				AddMaterial(material);
			}

			// �V�����}�e���A���𐶐�
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
			//�e�N�X�`���t�@�C�����ǂݍ���
			line_stream >> material->textureFilename;
		}
	}

	file.close();

	if (material)
	{
		// �}�e���A���o�^
		AddMaterial(material);
	}
}

void Model::LoadTextures()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	if (0 < m_materials.size())
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NumDescriptors = static_cast<UINT>(m_materials.size()); // �V�F�[�_�[���\�[�X�r���[�̐�
		result = s_dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));
		if (FAILED(result))
		{
			assert(0);
		}
	}
	// �f�X�N���v�^�T�C�Y���擾
	s_descriptorHandleIncrementSize = s_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ���_�C���f�b�N�X
	int textureIndex = 0;
	// �f�B���N�g���p�X
	string directoryPath = baseDirectory + m_name + "/";

	for (auto& m : m_materials)
	{
		Material* material = m.second.get();

		// �e�N�X�`������
		if (material->textureFilename.size() > 0)
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);
		}
		else
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, s_descriptorHandleIncrementSize);
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(baseDirectory, cpuDescHandleSRV, gpuDescHandleSRV);
		}
		textureIndex++;
	}
}
