#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Camera.h"
#include "GameScene.h"
#include "ParticleManager.h"
#include "SafeDelete.h"

class MyGame
{
private: //�|�C���^�u����
	WinApp* win = nullptr;
	Input* input = nullptr;
	Camera* camera = nullptr;
	Audio* audio = nullptr;
	DirectXCommon* dx_cmd = nullptr;
	GameScene* game_scene = nullptr;

private: //���̑��ϐ�
	bool end_request = false;

public: //�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I�����N�G�X�g�̐���
	/// </summary>
	bool IsEndRequest() { return end_request; }
};