#include "OnoGame.h"
#include "SceneFactory.h"
#include "Sprite.h"
#include "Object3d.h"

void OnoGame::LoadResources()
{
	// �X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/Reticle.png");
	Sprite::LoadTexture(2, L"Resources/NumberText.png");
	Sprite::LoadTexture(3, L"Resources/HPFrame.png");
	Sprite::LoadTexture(4, L"Resources/HPGauge.png");
	Sprite::LoadTexture(5, L"Resources/BoosterFrame.png");
	Sprite::LoadTexture(6, L"Resources/BoosterGauge.png");
	Sprite::LoadTexture(7, L"Resources/Monitor.png");
	Sprite::LoadTexture(8, L"Resources/GameOverScreen.png");
	Sprite::LoadTexture(9, L"Resources/BulletNumFrame.png");
	Sprite::LoadTexture(10, L"Resources/Rader.png");
	Sprite::LoadTexture(11, L"Resources/RaderEnemy.png");
	Sprite::LoadTexture(12, L"Resources/startScreen.png");
	Sprite::LoadTexture(13, L"Resources/EndScreen.png");

	// OBJ���f���ǂݍ���
	Object3d::LoadModel(0, "Desert", true);
	Object3d::LoadModel(1, "SkyWall");
	Object3d::LoadModel(2, "Tank");
	Object3d::LoadModel(3, "Bullet", true);
	Object3d::LoadModel(4, "Leg");
	Object3d::LoadModel(5, "Body");
	Object3d::LoadModel(6, "Arm");

	// FBX���f���ǂݍ���

}

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�V�[���̏�����
	scene_factory.reset(new SceneFactory);
	SceneManager::GetInstance()->SetSceneFactory(scene_factory.get());
	//�V�[���Z�b�g
	SceneManager::GetInstance()->ChangeScene("GAME");

	// ���\�[�X�̃��[�h
	LoadResources();
}

void OnoGame::Finalize()
{
	//���N���X�̏�����
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//���N���X�̍X�V
	FrameWork::Update();
}

void OnoGame::Draw()
{
	//���N���X�̍X�V
	FrameWork::Draw();
}