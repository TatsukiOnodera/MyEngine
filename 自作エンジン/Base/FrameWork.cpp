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
	input->Initialize(win);

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

	//�X�v���C�g�ÓI������
	if (!Sprite::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//�I�u�W�F�N�g�ÓI������
	if (!Object3d::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//�p�[�e�B�N���ÓI������
	if (!ParticleManager::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//���C�g�ÓI������
	if (!Light::StaticInitialize(dx_cmd->GetDev()))
	{
		assert(0);
	}

	//FBX�I�u�W�F�N�g�ÓI������
	if (!FbxObject3d::StaticInitialize(dx_cmd->GetDev()))
	{
		assert(0);
	}
}

void FrameWork::Finalize()
{
	//�V�[���t�@�N�g���[�J��
	FbxLoader::GetInstance()->Finalize();
	safe_delete(scene_factory);

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
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	dx_cmd->PreDraw();

	SceneManager::GetInstance()->Draw();

	dx_cmd->PostDraw();
}