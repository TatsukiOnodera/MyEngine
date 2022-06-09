#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Camera.h"
#include "Object3d.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "Light.h"
#include "FbxObject3d.h"
#include "PostEffect.h"

#include <memory>

class FrameWork
{
protected: //�|�C���^�u����
	WinApp* win = nullptr;
	Input* input = nullptr;
	DirectXCommon* dx_cmd = nullptr;
	bool end_request = false;
	std::unique_ptr<AbstractSceneFactory> scene_factory = nullptr;
	std::unique_ptr<PostEffect> postEffect = nullptr;

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