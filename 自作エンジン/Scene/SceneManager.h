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

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	static SceneManager* GetInstance();

private: //�C���X�g���N�^
	SceneManager() = default;
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator = (const SceneManager&) = delete;

public: //�����o�֐�
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