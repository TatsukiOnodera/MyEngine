#include "TitleScene.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>

using namespace DirectX;
using namespace Microsoft::WRL;

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();

	//スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/startScreen.png");

	//前景スプライト
	debugText.Initialize(fontNumber);

	//パーティクル
	//particle.reset(ParticleManager::Create());

	//スプライト
	startScreen.reset(Sprite::Create(1));

	//オブジェクト

	//パラメーター
	ResetVariable();

	//オーディオ
	//audio->Initialize();
}

void TitleScene::ResetVariable()
{
	
}

void TitleScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//各描画
	//DrawBackSprite(cmdList);
	DrawObject(cmdList);
	//DrawParticles(cmdList);
	//DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void TitleScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void TitleScene::DrawObject(ID3D12GraphicsCommandList* cmdList)
{
	//オブジェクト描画
	Object3d::PreDraw(cmdList);



	Object3d::PostDraw();

	//スプライト描画
	Sprite::PreDraw(cmdList);

	startScreen->Draw();

	Sprite::PostDraw();

	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	

	FbxObject3d::PostDraw();
}

void TitleScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	//UI描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void TitleScene::DrawParticles(ID3D12GraphicsCommandList* cmdList)
{
	//パーティクル描画
	ParticleManager::PreDraw(cmdList);

	//particle->Draw();

	ParticleManager::PostDraw();
}

void TitleScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//デバッグテキスト描画
	debugText.Draw(cmdList);
}