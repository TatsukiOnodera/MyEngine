#pragma once
#include "BaseScene.h"

class SceneManager
{
private:
	//���̃V�[��
	BaseScene* scene = nullptr;
	BaseScene* next_scene = nullptr;

public:
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//���̃V�[�����Z�b�g
	void SetNextScene(BaseScene* next_scene) { this->next_scene = next_scene; }
};