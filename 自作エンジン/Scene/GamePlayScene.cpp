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
	light->SetLightDir({-5, -5, 0, 0});
	Object3d::SetLight(light.get());

	//スプライト
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJオブジェクト
	chr.reset(Object3d::Create("chr_sword"));
	player.reset(Object3d::Create("chr_sword"));
	bullet.emplace_back(Object3d::Create("bullet", true));
	bulletInfo.emplace_back(new BulletInfo);
	for (int i = 0; i < FIN; i++)
	{
		wall[i].reset(Object3d::Create("Wall"));
	}

	//FBXオブェクト
	//fbxObject.reset(FbxObject3d::CreateFBXObject("cube"));

	//パラメーター
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	frame = 0;

	gravity = 9.8f;

	angleX = 0;
	angleY = 0;

	chr->SetPosition({ 0, -100, 0 });
	chr->SetRotation({ 0, 180, 0 });
	chr->SetScale({ 3, 3, 3 });
	chr->Update();

	player->SetPosition({ 0, -100, -10 });
	player->SetScale({ 2, 2, 2 });
	player->Update();

	for (int i = 0; i < bullet.size(); i++)
	{
		bulletInfo[i]->is_alive = false;
		bulletInfo[i]->vec = { 0, 0, 0 };
		bulletInfo[i]->deleteTimer = 0;
	}

	for (int i = 0; i < FIN; i++)
	{
		float size = 100;
		XMFLOAT3 pos = { 0, 0, 0 };
		XMFLOAT3 rot = { 0, 0, 0 };
		XMFLOAT3 scale = { size, size, size };
		if (i == FRONT)
		{
			pos = { 0, 0, size };
			rot = { 0, 180, 0 };
		} 
		else if (i == BACK)
		{
			pos = { 0, 0, -size };
			rot = { 0, 0, 0 };
		}
		else if (i == RIGHT)
		{
			pos = { size, 0, 0 };
			rot = { 0, -90, 0 };
		}
		else if (i == LEFT)
		{
			pos = { -size, 0, 0 };
			rot = { 0, 90, 0 };
		}
		else if (i == UP)
		{
			pos = { 0, size, 0 };
			rot = { 90, 0, 0 };
		}
		else if (i == DOWN)
		{
			pos = { 0, -size, 0 };
			rot = { -90, 0, 0 };
		}
		wall[i]->SetPosition(pos);
		wall[i]->SetRotation(rot);
		wall[i]->SetScale(scale);
		wall[i]->Update();
	}

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 5, -10 });
	camera->Update();
}

void GamePlayScene::Update()
{
	//時期の座標とベクトル初期化
	XMFLOAT3 pPos = player->GetPosition();
	XMFLOAT3 pVec = { 0, 0, 0 };

	//自機移動
	{
		//移動
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			pVec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.25f;
		}
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
		{
			pVec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.25f;
		}

		//ジャンプ
		if (input->PushKey(DIK_SPACE))
		{
			pVec.y += 0.25f;
		} 
		else
		{
			frame++;
			pVec.y -= gravity * powf(static_cast<float>(frame) / 60, 2);

			if (pPos.y + pVec.y < -100)
			{
				pVec.y = 0;
				frame = 0;
			}
		}

		//セット
		pPos = camera->ConvertWindowPos(pPos, pVec, angleY);
		player->SetPosition(pPos);
		player->SetRotation({ 0, angleY, 0 });
	}

	//攻撃
	{
		//発射
		if (input->TriggerKey(DIK_RETURN))
		{
			//すべて使われているか
			bool noBullet = true;

			//配列を調べる
			for (int i = 0; i < bullet.size(); i++)
			{
				//使われていないものがある
				if (bulletInfo[i]->is_alive == false)
				{
					//使っている
					noBullet = false;

					//フラグ
					bulletInfo[i]->is_alive = true;

					//移動の長さを1にする 
					bulletInfo[i]->vec = GetVectorAtoB(pPos, camera->GetEye());

					//セット
					bullet[i]->SetPosition(pPos);
					bullet[i]->Update();
					break;
				}
			}

			//すべて使われているなら新しく作る
			if (noBullet == true)
			{
				//生成
				bullet.emplace_back(Object3d::Create("bullet", true));
				bulletInfo.emplace_back(new BulletInfo);

				//フラグ
				bulletInfo.back()->is_alive = true;

				//移動の長さを1にする
				bulletInfo.back()->vec = GetVectorAtoB(pPos, camera->GetEye());

				//セット
				bullet.back()->SetPosition(pPos);
				bullet.back()->Update();
			}
		}
	}

	//カメラ
	{
		//左右に向く
		if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			angleY += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 2;

			if (angleY > 360)
			{
				angleY -= 360;
			}
		}
		//上下に向く
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
		{
			angleX += (input->PushKey(DIK_DOWN) - input->PushKey(DIK_UP)) * 2;

			if (angleX > 360)
			{
				angleX -= 360;
			}
		}

		//セット
		XMFLOAT3 eye = camera->FollowUpCamera(pPos, { 0, 1, -10 }, angleX, angleY);
		camera->SetEye(eye);
	}

	//弾
	{
		for (int i = 0; i < bullet.size(); i++)
		{
			//使われているなら
			if (bulletInfo[i]->is_alive == true)
			{
				XMFLOAT3 bPos = bullet[i]->GetPosition();
				bPos.x += bulletInfo[i]->vec.x;
				bPos.y += bulletInfo[i]->vec.y;
				bPos.z += bulletInfo[i]->vec.z;
				bullet[i]->SetPosition(bPos);

				bulletInfo[i]->deleteTimer++;
				if (bulletInfo[i]->deleteTimer > 10)
				{
					bulletInfo[i]->deleteTimer = 0;
					bulletInfo[i]->is_alive = false;
				}
			}
		}
	}

	//リセット
	if (input->TriggerKey(DIK_END))
	{
		ResetVariable();
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
	AnyDraw(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::AnyDraw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	chr->Draw();
	player->Draw();
	for (int i = 0; i < bullet.size(); i++)
	{
		if (bulletInfo[i]->is_alive == true)
		{
			bullet[i]->Draw();
		}
	}
	for (int i = 0; i < FIN; i++)
	{
		wall[i]->Draw();
	}

	Object3d::PostDraw();

	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	//fbxObject->Draw();

	FbxObject3d::PostDraw();

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

XMFLOAT3 GamePlayScene::GetVectorAtoB(XMFLOAT3 objectA, XMFLOAT3 objectB)
{
	return { objectA.x - objectB.x, objectA.y - objectB.y, objectA.z - objectB.z };
}

float GamePlayScene::GetLength(XMFLOAT3 objectA, XMFLOAT3 objectB)
{
	return sqrtf(powf(objectA.x - objectB.x, 2) + powf(objectA.y - objectB.y, 2) + powf(objectA.z - objectB.z, 2));
}
