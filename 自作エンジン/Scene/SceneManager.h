#pragma once
#include "BaseScene.h"

/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager
{
private: //�|�C���^�u����
	//���̃V�[��
	BaseScene* scene = nullptr;
	//���̃V�[��
	BaseScene* next_scene = nullptr;

public:
	~SceneManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���̃V�[���Z�b�g
	/// </summary>
	void SetNextScene(BaseScene* next_scene) { this->next_scene = next_scene; };
};