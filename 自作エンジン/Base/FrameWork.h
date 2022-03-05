#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Camera.h"
#include "Object3d.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include <SafeDelete.h>
#include "SceneManager.h"

class FrameWork
{
protected: //�|�C���^�u����
	WinApp* win = nullptr;
	Input* input = nullptr;
	Camera* camera = nullptr;
	Audio* audio = nullptr;
	DirectXCommon* dx_cmd = nullptr;
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
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();
};