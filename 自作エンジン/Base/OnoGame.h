#pragma once
#include "FrameWork.h"
#include "SceneManager.h"

class GamePlayScene;
class GameTitleScene;

class OnoGame : public FrameWork
{
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