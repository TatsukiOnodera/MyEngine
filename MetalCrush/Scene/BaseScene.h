#pragma once

// �O���錾
class SceneManager;

/// <summary>
/// �V�[�����
/// </summary>
class BaseScene
{
protected: //�|�C���^�u����
	//�V�[���}�l�[�W���[�i�؂�Ă���j
	SceneManager* scene_manager = nullptr;

public:
	virtual ~BaseScene() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;
};