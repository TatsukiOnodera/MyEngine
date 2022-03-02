#include "FrameWork.h"

void FrameWork::Run()
{
	//�Q�[��������
	Initialize();

	while (1)  // �Q�[�����[�v
	{
		//�Q�[�����t���[���X�V
		Update();

		//�Q�[���I�����N�G�X�g
		if (end_request)
		{
			break;
		}

		//�Q�[���`��
		Draw();
	}

	//�Q�[���I��
	Finalize();
}

void FrameWork::Initialize()
{
	//windows�A�v���P�[�V�����̏�����
	win = new WinApp;
	win->CreateGameWindow();

	//Input������
	input = Input::GetInstance();
	input->Initialize(win->GetWindowInstance(), win->GetHWND());

	//�J����������
	camera = Camera::GetInstance();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//�I�[�f�B�I������
	audio = Audio::GetInstance();

	// DirectX����������
	dx_cmd = DirectXCommon::GetInstance();
	if (!dx_cmd->Initialize(win))
	{
		assert(0);
	}

	//Sprite������
	if (!Sprite::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//Object3d������
	if (!Object3d::StaticInitialize(dx_cmd->GetDev(), camera, WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//�p�[�e�B�N��������
	if (!ParticleManager::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//�V�[���}�l�[�W���[����
	scene_manager = new SceneManager;
}

void FrameWork::Finalize()
{
	//�J��
	safe_delete(scene_manager);

	// �E�B���h�E�N���X��o�^����
	win->TerminateGameWindow();
	safe_delete(win);
}

void FrameWork::Update()
{
	//�I���̃��N�G�X�g
	if (win->ProcessMessage() || input->PushKey(DIK_ESCAPE))
	{
		end_request = true;
	}

	//�L�[�̏��擾
	input->Update();

	//�V�[���̍X�V
	scene_manager->Update();
}

void FrameWork::Draw()
{
	dx_cmd->PreDraw();

	scene_manager->Draw();

	dx_cmd->PostDraw();
}