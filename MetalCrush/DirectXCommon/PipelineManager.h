#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>

class PipelineManager
{
public: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //�����o�ϐ�
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> m_pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> m_rootSignature;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="shaderName">�V�F�[�_�[��</param>
	PipelineManager(ID3D12Device* dev, std::wstring shaderName);

	/// <summary>
	/// �t�H���V�F�[�_�[
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="shaderName">�V�F�[�_�[��</param>
	void CreateShaderPipeline(ID3D12Device* dev, std::wstring shaderName);

	/// <summary>
	/// �p�C�v���C���X�e�[�g���擾
	/// </summary>
	/// <returns>�p�C�v���C���X�e�[�g</returns>
	ID3D12PipelineState *GetPipelineState() { return m_pipelineState.Get(); }

	/// <summary>
	/// ���[�g�V�O�l�`�����擾
	/// </summary>
	/// <returns>���[�g�V�O�l�`��</returns>
	ID3D12RootSignature *GetRootSignature() { return m_rootSignature.Get(); }
};
