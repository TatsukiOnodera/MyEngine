#include "TitleScene.h"
#include <time.h>
#include <stdlib.h>
#include "SafeDelete.h"

using namespace DirectX;
using namespace Microsoft::WRL;

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{
	safe_delete(particle);
}

void TitleScene::Initialize(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera)
{
	this->dx_cmd = dx_cmd;
	this->input = input;
	this->audio = sound;
	this->camera = camera;

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