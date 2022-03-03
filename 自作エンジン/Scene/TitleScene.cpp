#include "TitleScene.h"
#include "GamePlayScene.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>

using namespace DirectX;
using namespace Microsoft::WRL;

TitleScene::~TitleScene()
{
	safe_delete(particle);
}

void TitleScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();

	//スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");

	//前景スプライト
	debugText.Initialize(fontNumber);

	//スプライト

	//オブジェクト

	//パラメーター
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void TitleScene::ResetVariable()
{
	
}

void TitleScene::Update()
{
	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		scene_manager->SetNextScene(scene);
	}
}

void TitleScene::Draw()
{
	DrawBackSprite();
	DrawObject();
	DrawParticles();
	DrawUI();
	DrawDebugText();
}

void TitleScene::DrawBackSprite()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void TitleScene::DrawObject()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	

	Object3d::PostDraw();

	//スプライト描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void TitleScene::DrawUI()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//UI描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void TitleScene::DrawParticles()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//パーティクル描画
	//particle->Draw(cmdList);
}

void TitleScene::DrawDebugText()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//スプライト描画
	Sprite::PreDraw(cmdList);

	//debugText.Draw();

	Sprite::PostDraw();
}