#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

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
	//�V�[���t�@�N�g���[
	AbstractSceneFactory* scene_factory = nullptr;

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
	/// <param name="scene_name">�V�[����</param>
	void ChangeScene(const std::string& scene_name);

	void SetSceneFactory(AbstractSceneFactory* scene_factory) { this->scene_factory = scene_factory; };
};