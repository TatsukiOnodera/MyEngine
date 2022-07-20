#include "GamePlayScene.h"
#include "SceneManager.h"

#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <fbxsdk.h>

using namespace DirectX;
using namespace Microsoft::WRL;

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();
	
	//スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//前景スプライト
	debugText.Initialize(fontNumber);

	//ライト生成
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({0, 0, 1, 0});
	Object3d::SetLight(light.get());

	//スプライト
	

	//オブジェクト
	ball.reset(Object3d::Create("Circle", true));
	line.reset(Object3d::Create("line", true));

	//パラメーター
	camera->SetEye({ 0, 0, -200 });
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	start = { 0, 0, 0 };
	end = { 0, 0, 0 };
	r = 10;

	line->SetRotation({90, 0, -45});
	ball->SetRotation({90, 0, 0});

	line->SetScale({ 2.5, 2.5, 2.5 });
	ball->SetScale({ 10, 10, 10 });

	line->Update();
	ball->Update();
}

void GamePlayScene::Update()
{
	XMFLOAT3 pos = line->GetPosition();

	pos.x += input->PushKey(DIK_D) - input->PushKey(DIK_A) * 2;
	pos.y += input->PushKey(DIK_W) - input->PushKey(DIK_S) * 2;

	float len = static_cast<float>((2.5 * 10) / sqrt(2));

	start = { pos.x - len, pos.y - len, 0 };
	end = { pos.x + len, pos.y + len, 0 };

	line->SetPosition(pos);

	float distance = ((0 - start.x) * (end.y - start.y) - (end.x - start.x) * (0 - start.y)) / static_cast<float>(sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2)));

	if (fabsf(distance) <= 10)
	{
		float dotA = (0 - start.x) * (end.x - start.x) + (0 - start.y) * (end.y - start.y);
		float dotB = (0 - end.x) * (end.x - start.x) + (0 - end.y) * (end.y - start.y);

		if (dotA * dotB <= 0.0f)
		{
			ball->SetColor(XMFLOAT4(1, 0, 0, 1));
		}
		else if (CalculationVectorLength(XMFLOAT3(0 - start.x, 0 - start.y, 0)) < r || CalculationVectorLength(XMFLOAT3(0 - end.x, 0 - end.y, 0)) < r)
		{
			ball->SetColor(XMFLOAT4(1, 0, 0, 1));
		}
		else
		{
			ball->SetColor(XMFLOAT4(1, 1, 1, 1));
		}
	}
	else
	{
		ball->SetColor(XMFLOAT4(1, 1, 1, 1));
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		int c = 0;
	}
}

void GamePlayScene::Draw()
{
	//カメラとライトの更新
	light->Update();
	camera->Update();

	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//各描画
	//DrawBackSprite(cmdList);
	Draw(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	ball->Draw();
	line->Draw();

	Object3d::PostDraw();

	//スプライト描画
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	//UI描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//パーティクル描画
	particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//デバッグテキスト描画
	debugText.Draw(cmdList);
}

float GamePlayScene::CalculationVectorLength(XMFLOAT3 vec)
{
	return static_cast<float>(sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));
}
