#include "Player.h"

Player::~Player()
{

}

void Player::Initialize()
{
	m_player.reset(FbxObject3d::CreateFBXObject("Human"));

	m_isDash = false;
	m_add0 = 0;

	float playerSize = 0.5f;
	m_player->SetPosition({ 0, 0, -100 });
	m_player->SetRotation({ -90, 0, 0 });
	m_player->SetScale({ playerSize, playerSize, playerSize });
	m_player->Update();
}

void Player::Update()
{
	//移動
	XMFLOAT3 vec = { 0, 0, 0 };
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		//ダッシュ
		if (input->TriggerKey(DIK_SPACE))
		{
			isDash = true;
			add0 = 25;

			//アニメーション
			fbxObject->PlayAnimation(false);
		}

		//ベクトル
		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
		//加速するなら
		if (isDash)
		{
			vec.x *= add0;
			vec.z *= add0;

			add0 = add0 - 10;

			//加速度が0になったら
			if (add0 <= 0)
			{
				add0 = 0;
				isDash = false;
			}
		}
	} else
	{
		//アニメーション
		fbxObject->ResetAnimation();
	}
	//カメラを軸にした変換
	XMFLOAT3 pos = fbxObject->GetPosition();
	pos = camera->ConvertWindowPos(pos, vec);
	fbxObject->SetPosition(pos);
}

void Player::Draw()
{


	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	m_player->Draw();

	FbxObject3d::PostDraw();
}
