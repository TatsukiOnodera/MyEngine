#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <vector>
#include <unordered_map>

class Mesh
{
private: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

private: // �ÓI�����o�ϐ�
// �f�o�C�X
	static ID3D12Device* s_dev;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* dev);

private: // �����o�ϐ�
	// ���O
	std::string m_name;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> m_vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> m_indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> m_vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<uint32_t> m_indices;
	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<uint32_t, std::vector<uint32_t>> m_smoothData;
	// �}�e���A��
	Material* m_material = nullptr;

public: // �����o�֐�
	/// <summary>
	/// �o�b�t�@�̐���
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">���ߔ��s��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �G�b�W�������f�[�^�̒ǉ�
	/// </summary>
	/// <param name="indexPosition">���_�ʒu</param>
	/// <param name="indexVertex">���_���W</param>
	inline void AddSmoothData(uint32_t indexPosition, uint32_t indexVertex)
	{
		m_smoothData[indexPosition].emplace_back(indexVertex);
	}

	/// <summary>
	/// ���������ꂽ���_�@���̌v�Z
	/// </summary>
	void CalculateSmoothedVertexNormals();

public: // �A�N�Z�b�T
	/// <summary>
	/// ���O���擾
	/// </summary>
	/// <returns>���O</returns>
	inline const std::string& GetName() { return m_name; }

	/// <summary>
	/// ���O���Z�b�g
	/// </summary>
	/// <param name="name">���O</param>
	inline void SetName(const std::string& name)
	{
		m_name = name;
	}

	/// <summary>
	/// ���_�f�[�^�̒ǉ�
	/// </summary>
	/// <param name="vertex">���_�f�[�^</param>
	inline void AddVertex(const VertexPosNormalUv& vertex)
	{
		m_vertices.emplace_back(vertex);
	}

	/// <summary>
	/// ���_�C���f�b�N�X�̒ǉ�
	/// </summary>
	/// <param name="index">�C���f�b�N�X</param>
	inline void AddIndex(uint32_t index)
	{
		m_indices.emplace_back(index);
	}

	/// <summary>
	/// ���_�f�[�^�̐����擾
	/// </summary>
	/// <returns>���_�f�[�^�̐�</returns>
	inline size_t GetVertexCount() { return m_vertices.size(); }

	/// <summary>
	/// �w��̒��_�f�[�^�擾
	/// </summary>
	/// <param name="index">�w��̔ԍ�</param>
	/// <returns>���_�f�[�^</returns>
	inline const VertexPosNormalUv& GetVertices(uint32_t index) { return m_vertices[index]; }

	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	/// <returns>�}�e���A��</returns>
	inline Material* GetMaterial() { return m_material; }

	/// <summary>
	/// �}�e���A���̊��蓖��
	/// </summary>
	/// <param name="material">�}�e���A��</param>
	inline void SetMaterial(Material* material)
	{
		m_material = material;
	}

	/// <summary>
	/// ���_�o�b�t�@�擾
	/// </summary>
	/// <returns>���_�o�b�t�@</returns>
	inline const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return m_vbView; }

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�擾
	/// </summary>
	/// <returns>�C���f�b�N�X�o�b�t�@</returns>
	inline const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return m_ibView; }
};
