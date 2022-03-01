#pragma once
#include "FrameWork.h"

class GamePlayScene;
class TitleScene;

class OnoGame : public FrameWork
{
private: //�|�C���^�u����
	//GamePlayScene* game_scene = nullptr;
	TitleScene* scene = nullptr;

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