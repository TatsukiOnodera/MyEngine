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
		if ((s_input->PushKey(DIK_D) || s_input->PushKey(DIK_A) || s_input->PushKey(DIK_W) || s_input->PushKey(DIK_S)) && m_pos.y == -100 + 4.0f)
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
		}
		else
		{
			//アニメーション
			m_object->ResetAnimation();
		}

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
				m_object->ResetAnimation();
			}
		}

		// 重力
		if (s_input->PushKey(DIK_RSHIFT) && m_pos.y >= -100 + 4.0f)
		{
			pVec.y += 1.5;
		}
		pVec.y += -0.98;

		//カメラを軸にした変換
		m_pos = m_object->GetPosition();
		m_pos = s_camera->ConvertWindowPos(m_pos, pVec);

		//エフェクト
		if (effectTimer > 0)
		{
			effectTimer++;
		}
		
		//セット
		if (m_pos.x < -100 + 5)
		{
			m_pos.x = -100 + 5;
		}
		else if (100 - 5 < m_pos.x)
		{
			m_pos.x = 100 - 5;
		}
		if (m_pos.z < -100 + 5)
		{
			m_pos.z = -100 + 5;
		}
		else if (100 - 5 < m_pos.z)
		{
			m_pos.z = 100 - 5;
		}
		if (m_pos.y - 4.0f < -100)
		{
			m_pos.y = -100 + 4.0f;
		}
		m_object->SetPosition(m_pos);

		//姿勢制御
		XMFLOAT3 rot = {};
		rot.x = -90;
		rot.y += Camera::GetInstance()->GetAngle().y;
		m_object->SetRotation(rot);
	}
	else
	{
		XMFLOAT3 pos = m_object->GetPosition();
		pos.y -= 0.98f;
		if (pos.y < -100 + 4.0f)
		{
			pos.y = -100 + 4.0f;
		}
		m_object->SetPosition(pos);
	}
}

void Player::Draw()
{
	if (m_alive && effectTimer % 5 == 0)
	{
		if (effectTimer > 60)
		{
			effectTimer = 0;
		}
		m_object->Draw();
	}

	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCmdList());

	for (const auto& m : bullet)
	{
		m->Draw();
	}

	Object3d::PostDraw();
}

bool Player::bulletUpdate(XMFLOAT3 enemyPos)
{
	//弾の更新
	bool f = false;
	for (auto& m : bullet)
	{
		if (m->Update(enemyPos))
		{
			f = true;
		}
	}

	return f;
}

void Player::ShotBullet(XMFLOAT3 enemyPos)
{
	//弾の発射
	if (s_input->TriggerKey(DIK_RETURN) && effectTimer == 0 && m_pos.y >= -100 + 4.0f)
	{
		float bulletSpeed = 6;
		//すべて使われているか
		if (UsingAllBullet())
		{
			XMFLOAT3 len = { enemyPos.x - m_pos.x, enemyPos.y - m_pos.y, enemyPos.z - m_pos.z };
			len.x /= Length(enemyPos, m_pos);
			len.y /= Length(enemyPos, m_pos);
			len.z /= Length(enemyPos, m_pos);
			len.x *= bulletSpeed;
			len.y *= bulletSpeed;
			len.z *= bulletSpeed;
			bullet.emplace_back(Bullet::Create(m_pos, len, true));
		}
		else
		{
			for (auto& m : bullet)
			{
				if (m->GetAlive() == false)
				{
					XMFLOAT3 len = { enemyPos.x - m_pos.x, enemyPos.y - m_pos.y, enemyPos.z - m_pos.z };
					len.x /= Length(enemyPos, m_pos);
					len.y /= Length(enemyPos, m_pos);
					len.z /= Length(enemyPos, m_pos);
					len.x *= bulletSpeed;
					len.y *= bulletSpeed;
					len.z *= bulletSpeed;
					m->Initialize(m_pos, len, true);
					break;
				}
			}
		}
	}
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

const float Player::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

void Player::SetEffectTimer()
{
	effectTimer = 1;
}
