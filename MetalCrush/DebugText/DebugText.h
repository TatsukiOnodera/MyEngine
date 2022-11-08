#pragma once
#include "Sprite.h"

#include <string>

class DebugText
{
public: // �萔
	// �ő�\����
	static const int c_maxCharCount = 256;
	// ���̃T�C�Y
	static const int c_fontWidth = 9;
	// �c�̃T�C�Y
	static const int c_fontHeight = 18;
	// ��s�̒���
	static const int c_fontLineCount = 14;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texnumber">�e�N�X�`���̔ԍ�</param>
	void Initialize(const UINT texnumber);

	/// <summary>
	/// ��ʂɕ\������
	/// </summary>
	/// <param name="text">�\������</param>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="scale">�X�P�[��</param>
	void Print(const std::string& text, const float x, const float y, const float scale = 1.0f);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	// ������`�悷��X�v���C�g
	Sprite* sprites[c_maxCharCount] = {};
	// �w��̕���
	int spriteIndex = 0;
};