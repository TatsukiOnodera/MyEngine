#include "Player.h"
#include "DirectXCommon.h"

Input* Player::s_input = Input::GetInstance();
Camera* Player::s_camera = Camera::GetInstance();

Player::Player()
{
	
}

Player::~Player()
{

}

Player* Player::Create()
{
	Player* player = new Player;

	player->Initialize();

	return player;
}

void Player::Initialize()
{
	m_object.reset(FbxObject3d::CreateFBXObject("player"));

	m_alive = true;
	m_isDash = false;
	m_add0 = 0;

	assert(m_object);
	m_object->SetPosition(m_pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	if (m_alive)
	{
		XMFLOAT3 pVec = {};
		if (s_input->PushKey(DIK_D) || s_input->PushKey(DIK_A) || s_input->PushKey(DIK_W) || s_input->PushKey(DIK_S))
		{
			//ダッシュ
			if (s_input->TriggerKey(DIK_SPACE))
			{
				m_isDash = true;
				m_add0 = 7.5;

				//アニメーション
				m_object->PlayAnimation(false);
			}

			//ベクトル
			pVec.x += (s_input->PushKey(DIK_D) - s_input->PushKey(DIK_A)) * 0.75;
			pVec.z += (s_input->PushKey(DIK_W) - s_input->PushKey(DIK_S)) * 0.75;
			//加速するなら
			if (m_isDash)
			{
				pVec.x *= m_add0;
				pVec.z *= m_add0;

				m_add0 = m_add0 - 0.75;

				//加速度が0になったら
				if (m_add0 <= 0)
				{
					m_add0 = 0;
					m_isDash = false;
				}
			}
		}
		else
		{
			//アニメーション
			m_object->ResetAnimation();
		}

		//カメラを軸にした変換
		m_pos = m_object->GetPosition();
		m_pos = s_camera->ConvertWindowPos(m_pos, pVec);
		m_object->SetPosition(m_pos);

		//弾の発射
		if (s_input->TriggerKey(DIK_RETURN))
		{
			//すべて使われているか
			if (UsingAllBullet())
			{
				bullet.emplace_back(Bullet::Create(m_pos, { 0, 0, 1 }, true));
			}
			else
			{
				for (auto& m : bullet)
				{
					if (m->GetAlive() == false)
					{
						m->Initialize(m_pos, { 0, 0, 1 }, true);
						break;
					}
				}
			}
		}

		//弾の更新
		for (auto& m : bullet)
		{
			m->Update();
		}
	}
}

void Player::Draw()
{
	if (m_alive)
	{
		m_object->Draw();
	}

	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCmdList());

	for (const auto& m : bullet)
	{
		m->Draw();
	}

	Object3d::PostDraw();
}

bool Player::UsingAllBullet()
{
	for (const auto& m : bullet)
	{
		if (m->GetAlive() == false)
		{
			return false;
		}
	}

	return true;
}
