#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Camera.h"
#include "GameScene.h"
#include "ParticleManager.h"
#include "SafeDelete.h"

class FrameWork
{
protected: //�|�C���^�u����
	WinApp* win = nullptr;
	Input* input = nullptr;
	Camera* camera = nullptr;
	Audio* audio = nullptr;
	DirectXCommon* dx_cmd = nullptr;

public: //���̑��ϐ�
	bool end_request = false;

public:
	/// <summary>
	/// ���s
	/// </summary>
	void Run();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();
};