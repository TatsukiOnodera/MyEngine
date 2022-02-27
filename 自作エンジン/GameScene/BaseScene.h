#pragma once

//�O���錾
class SceneManager;

class BaseScene
{
public:
	BaseScene(SceneManager* scene_manager);

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

private:
	//�V�[���}�l�[�W���[�i�؂�Ă���j
	SceneManager* scene_manager = nullptr;
};