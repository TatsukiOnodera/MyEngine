#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
protected:
	//�e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j
	ComPtr<ID3D12Resource> texBuff;
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
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
};