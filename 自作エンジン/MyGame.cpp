#include "MyGame.h"

void MyGame::Initialize()
{
	//windows�A�v���P�[�V�����̏�����
	win = new WinApp;
	win->CreateGameWindow();

	//Input������
	input = new Input;
	input->Initialize(win->GetWindowInstance(), win->GetHWND());

	//�J����������
	camera = new Camera;
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//�I�[�f�B�I������
	audio = new Audio;

	// DirectX����������
	dx_cmd = new DirectXCommon();
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

	//�Q�[���V�[��������
	game_scene = new GameScene;
	game_scene->Initialize(dx_cmd, input, audio, camera);
}

void MyGame::Finalize()
{
	//�J��
	safe_delete(game_scene);
	safe_delete(dx_cmd);
	safe_delete(camera);
	safe_delete(input);
	safe_delete(audio);

	// �E�B���h�E�N���X��o�^����
	win->TerminateGameWindow();
	safe_delete(win);
}

void MyGame::Update()
{
	//�I�����N�G�X�g
	if (win->ProcessMessage() || input->PushKey(DIK_ESCAPE))
	{
		end_request = true;
	}

	//�L�[���擾
	input->Update();
	
	//�Q�[���V�[��
	game_scene->Update();
}

void MyGame::Draw()
{
	dx_cmd->PreDraw();

	game_scene->Draw();

	dx_cmd->PostDraw();
}