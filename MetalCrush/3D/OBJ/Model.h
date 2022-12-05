#pragma once
#include "PipelineManager.h"
#include "Mesh.h"

#include <string>
#include <vector>

class Model
{
private: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: // �萔
	// ��{�f�B���N�g��
	static const std::string baseDirectory;

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* s_dev;
	// �f�X�N���v�^�T�C�Y
	static UINT s_descriptorHandleIncrementSize;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// ���f������
	/// </summary>
	/// <param name="modelName">���f���̃p�X</param>
	/// <param name="smooting">�X���[�Y�V�F�[�f�B���O�̗L��</param>
	/// <returns>���f��</returns>
	static Model* Create(const std::string& modelName, const bool smoothing = false);

private: // �����o�ϐ�
	// �O���t�B�b�N�X�p�C�v���C��
	std::unique_ptr<PipelineManager> m_graphicsPipeline = nullptr;
	// ���O
	std::string m_name;
	// ���b�V���R���e�i
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_descHeap;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Model();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	~Model();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="modelName">���f���̃p�X</param>
	/// <param name="smooting">�X���[�Y�V�F�[�f�B���O�̗L��</param>
	void Initialize(const std::string& modelName, const bool smoothing);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�f�B���N�g���p�X</param>
	/// <param name="filename">�t�@�C����</param>
	void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	/// <param name="material">�}�e���A��</param>
	inline void AddMaterial(Material* material)
	{
		m_materials.emplace(material->name, material);
	}

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTextures();

public: // �A�N�Z�b�T
	/// <summary>
	/// ���b�V���z��擾
	/// </summary>
	/// <returns>���b�V���z��</returns>
	inline const std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_meshes; }

	/// <summary>
	/// �p�C�v���C���X�e�[�g���擾
	/// </summary>
	/// <returns>�p�C�v���C���X�e�[�g</returns>
	ID3D12PipelineState* GetPipelineState() { return m_graphicsPipeline->GetPipelineState(); }

	/// <summary>
	/// ���[�g�V�O�l�`�����擾
	/// </summary>
	/// <returns>���[�g�V�O�l�`��</returns>
	ID3D12RootSignature* GetRootSignature() { return m_graphicsPipeline->GetRootSignature(); }
};