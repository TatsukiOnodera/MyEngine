#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public: //�ÓI�����o�֐�
	//��ʃN���A�J���[
	static const float clearColor[4];

protected: //�����o�ϐ�
	//�e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j
	ComPtr<ID3D12Resource> texBuff[2];
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//�f�v�X�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�O���t�B�b�N�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��R�}���h
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
};