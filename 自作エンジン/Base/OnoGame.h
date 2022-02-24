#pragma once
#include "FrameWork.h"

class OnoGame : public FrameWork
{
private: //�|�C���^�u����
	GameScene* game_scene = nullptr;

public: //�֐�
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