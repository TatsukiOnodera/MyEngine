#pragma once
#include "FrameWork.h"

class OnoGame : public FrameWork
{
public: //�֐�
	/// <summary>
	/// ���\�[�X�̓ǂݍ���
	/// </summary>
	void LoadResources();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
};