#include "Model.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream> //�t�@�C�����o��
#include <sstream> //������ɑ΂�����o��
#include <string> //������
#include <vector> //���_�f�[�^���܂Ƃ߂�̂Ɏg��
#include <map>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Model::s_dev = nullptr;
UINT Model::s_descriptorHandleIncrementSize = 0;

void Model::StaticInitialize(ID3D12Device* dev)
{
	// nullptr�`�F�b�N
	assert(dev);

	Model::s_dev = dev;
}

Model *Model::CreateModel(const std::string& modelName, bool smooting)
{
	//�C���X�^���X����
	Model* model = new Model;

	//�O���t�B�b�N�X�p�C�v���C������
	model->m_graphicsPipeline.reset(new PipelineManager(s_dev));

	//���f���f�[�^�ǂݍ���
	model->InitializeModel(modelName, smooting);

	//�e�N�X�`���̐���1��葽���Ȃ�
	if (model->m_graphicsPipeline->GetTexNum() == 2)
	{
		//�}�X�N�e�N�X�`�����擾
		model->LoadTextureName("Resources/Default/", "red1x1.png");
	}
	else if (model->m_graphicsPipeline->GetTexNum() == 3)
	{
		//�T�u�e�N�X�`�����擾
		model->LoadTextureName("Resources/Default/", "white1x1.png");
		//�}�X�N�e�N�X�`�����擾
		model->LoadTextureName("Resources/Default/", "red1x1.png");
	}

	//�e�N�X�`���̓ǂݍ���
	model->LoadTexture();

	return model;
}

void Model::Initialize()
{
	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));
}

void Model::Update()
{
	HRESULT result;

	// �萔�o�b�t�@�փf�[�^�]��
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
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&m_ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { m_descHeap.Get() };
	//�f�X�g���N�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(1, m_constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	for (int i = 0; i < m_graphicsPipeline->GetTexNum(); i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(3 + i, m_gpuDescHandleSRV[i]);
	}

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)m_indices.size(), 1, 0, 0, 0);
}

void Model::SetGraphicsPipeline(const int shaderType)
{
	//null�`�F�b�N
	assert(m_graphicsPipeline);

	//�w��̃V�F�[�_�[����
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

	//�e�N�X�`���̐���1��葽���Ȃ�
	if (m_graphicsPipeline->GetTexNum() == 2)
	{
		//�}�X�N�e�N�X�`�����擾
		LoadTextureName("Resources/Default/", "red1x1.png");
	}
	else if (m_graphicsPipeline->GetTexNum() == 3)
	{
		//�T�u�e�N�X�`�����擾
		LoadTextureName("Resources/Default/", "black1x1.png");
		//�}�X�N�e�N�X�`�����擾
		LoadTextureName("Resources/Default/", "red1x1.png");
	}

	//�e�N�X�`���̓ǂݍ���
	LoadTexture();
}

void Model::SetSubTexture(const std::string& filename)
{
	if (!(m_graphicsPipeline->GetTexNum() == 3) || !(textureName.size() == 3))
	{
		return;
	}

	//�T�u�e�N�X�`�����擾
	textureName[1] = filename;

	//�e�N�X�`���̓ǂݍ���
	LoadTexture();
}

void Model::SetMaskTexture(const std::string& filename)
{
	if (textureName.size() == 2 || textureName.size() == 3)
	{
		if (m_graphicsPipeline->GetTexNum() == 2)
		{
			//�}�X�N�e�N�X�`�����擾
			textureName[1] = filename;
		} 
		else if (m_graphicsPipeline->GetTexNum() == 3)
		{
			//�}�X�N�e�N�X�`�����擾
			textureName[2] = filename;
		}
	}
	else
	{
		return;
	}

	//�e�N�X�`���̓ǂݍ���
	LoadTexture();
}

void Model::InitializeModel(const std::string& modelName, bool smooting)
{
	HRESULT result = S_FALSE;

	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	const string filename = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	vector<XMFLOAT3> positions; //���_���W
	vector<XMFLOAT3> normals; //�@���x�N�g��
	vector<XMFLOAT2> texcoords; //�e�N�X�`��UV
	unsigned short texVertex = 0; //�e�N�X�`���̗v�f��

	//1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̕��������Ȃ璸�_���W
		if (key == "v") {
			//X, Y, Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		if (key == "vt") {
			//U, V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn") {
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
			int v_num = 0;
			while (getline(line_stream, index_string, ' ')) {
				//���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); //�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur); //�X���b�V�����΂�
				index_stream >> indexNormal;
				//���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				m_vertices.emplace_back(vertex);
				//�G�b�W�������p�̃f�[�^��ǉ�
				if (smooting == true)
				{
					AddSmoothData(indexPosition, (unsigned short)m_vertices.size() - 1);
				}
				//�C���f�b�N�X�f�[�^�ɒǉ�
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
				//���_�J�E���g
				v_num++;
				//���̃e�N�X�`���̒��_�ǉ�
				texVertex++;
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
	}
	file.close();

	if (smooting == true)
	{
		CalculateSmoothedVertexNormals();
	}

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * m_vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * m_indices.size());

	// ���_�o�b�t�@����
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

	// �C���f�b�N�X�o�b�t�@����
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

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(m_vertices.begin(), m_vertices.end(), vertMap);
		m_vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = m_indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(m_indices.begin(), m_indices.end(), indexMap);
		m_indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	m_vbView.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = sizeVB;
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
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
		//�e�ʂ̋��ʒ��_�R���N�V����
		std::vector<unsigned short>& v = itr->second;
		//�S���_�̖@���𕽋ω�����
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(m_vertices[index].normal.x, m_vertices[index].normal.y, m_vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//���ʖ@�����g�p���邷�ׂĂ̒��_�f�[�^�ɏ�������
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

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = m_graphicsPipeline->GetTexNum(); // �V�F�[�_�[���\�[�X�r���[�̐�
	result = s_dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));//����
	if (FAILED(result)) {
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	s_descriptorHandleIncrementSize = s_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < m_graphicsPipeline->GetTexNum(); i++)
	{
		// WIC�e�N�X�`���̃��[�h
		TexMetadata metadata{};
		ScratchImage scratchImg{};

		//�t�@�C���p�X������
		string filepath = textureName[i];

		//���j�R�[�h������ɕϊ�����
		wchar_t wfilepath[128];
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

		//�e�N�X�`���̃��[�h
		result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
		if (FAILED(result))
		{
			assert(0);
		}

		const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o
		assert(img);

		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels
		);

		// �e�N�X�`���p�o�b�t�@�̐���
		result = s_dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
			nullptr,
			IID_PPV_ARGS(&m_texbuff[i]));
		if (FAILED(result)) 
		{
			assert(0);
		}

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = m_texbuff[i]->WriteToSubresource(
			0,
			nullptr, // �S�̈�փR�s�[
			img->pixels,    // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch); // 1���T�C�Y
		if (FAILED(result)) 
		{
			assert(0);
		}

		// �V�F�[�_���\�[�X�r���[�쐬
		m_cpuDescHandleSRV[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), i, s_descriptorHandleIncrementSize);
		m_gpuDescHandleSRV[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), i, s_descriptorHandleIncrementSize);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
		D3D12_RESOURCE_DESC resDesc = m_texbuff[i]->GetDesc();

		srvDesc.Format = resDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
		srvDesc.Texture2D.MipLevels = 1;

		s_dev->CreateShaderResourceView(m_texbuff[i].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
			&srvDesc, //�e�N�X�`���ݒ���
			m_cpuDescHandleSRV[i]);
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
			//�}�e���A�����ǂݍ���
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
			//�e�N�X�`���t�@�C�����ǂݍ���
			line_stream >> m_material.textureFilename;
			//�e�N�X�`���ǂݍ���
			LoadTextureName(directoryPath, m_material.textureFilename);
		}
	}
	//�e�N�X�`����ǂݍ���ł��Ȃ�
	if (m_material.textureFilename == "defaultTexture")
	{
		//�e�N�X�`���t�@�C�����ǂݍ���
		m_material.textureFilename = "white1x1.png";
		//�e�N�X�`���ǂݍ���
		LoadTextureName("Resources/Default/", m_material.textureFilename);
	}

	file.close();
}

void Model::LoadTextureName(const std::string& directoryPath, const std::string& filename)
{
	textureName.emplace_back(directoryPath + filename);
}
