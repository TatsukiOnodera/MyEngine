#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public: //�ÓI�����o�֐�
	//��ʃN���A�J���[
	static const float clearColor[4];

protected: //�����o�ϐ�
	//�e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j
	ComPtr<ID3D12Resource> texBuff;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//�f�v�X�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

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