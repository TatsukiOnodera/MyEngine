#include "GameTitleScene.h"
#include "SafeDelete.h"

using namespace DirectX;
using namespace Microsoft::WRL;

GameTitleScene::GameTitleScene(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera, SceneManager* scene_manager) : BaseScene(scene_manager)
{
	this->dx_cmd = dx_cmd;
	this->input = input;
	this->audio = sound;
	this->camera = camera;
}

GameTitleScene::~GameTitleScene()
{
	safe_delete(particle);
	safe_delete(demo_back);
	safe_delete(obj);
}

void GameTitleScene::Initialize()
{
	//スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//前景スプライト
	debugText.Initialize(fontNumber);

	//スプライト
	demo_back = Sprite::CreateSprite(1);

	//オブジェクト
	obj = Object3d::Create("Bullet");

	//パラメーター
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GameTitleScene::ResetVariable()
{
	obj->SetPosition({10.0f, 0.0f, 0.0f});
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	obj->SetColor({1, 1, 1, 0.8f});
	obj->Update();
}

void GameTitleScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{

	}

	obj->Update();
}

void GameTitleScene::Draw()
{
	DrawBackSprite(); //前景スプライト
	DrawObject(); //オブジェクト＆スプライト描画
	//DrawParticles(); //パーティクル描画
	//DrawUI(); //UI描画

	//DrawDebugText(); //デバッグテキスト描画
}

void GameTitleScene::DrawBackSprite()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GameTitleScene::DrawObject()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	obj->Draw();

	Object3d::PostDraw();

	//スプライト描画
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GameTitleScene::DrawParticles()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//パーティクル描画
	particle->Draw(cmdList);
}

void GameTitleScene::DrawUI()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//UI描画
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GameTitleScene::DrawDebugText()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//スプライト描画
	Sprite::PreDraw(cmdList);

	debugText.Draw();

	Sprite::PostDraw();
}