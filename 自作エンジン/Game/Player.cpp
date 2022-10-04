#include "DirectXCommon.h"

#include "Player.h"

Camera* Player::s_camera = Camera::GetInstance();

Player::Player()
{
	m_object.reset(FbxObject3d::CreateFBXObject("player"));

	Initialize();
}

Player::~Player()
{
	
}

void Player::Initialize()
{
	if (m_object == nullptr)
	{
		assert(0);
	}

	m_alive = true;

	m_pos = { 0, 0, -50 };
	m_vel = { 0, 0, 0 };
	m_addSpeed = 0.1f;
	m_maxSpeed = 0.6f;
	m_dashSpeed = 0;
	m_gravityTime = 0;

	m_isDash = false;

	m_object->SetPosition(m_pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	if (m_alive == true)
	{
		// 加速度
		XMFLOAT3 add = {};
		// 移動
		if (s_input->LeftStickAngle().x != 0 || s_input->LeftStickAngle().y != 0)
		{
			add.x += s_input->LeftStickAngle().x * m_addSpeed;
			add.z += s_input->LeftStickAngle().y * m_addSpeed;
		}
		else if (s_input->PushKey(DIK_D) || s_input->PushKey(DIK_A) || s_input->PushKey(DIK_W) || s_input->PushKey(DIK_S))
		{
			add.x += (s_input->PushKey(DIK_D) - s_input->PushKey(DIK_A)) * m_addSpeed;
			add.z += (s_input->PushKey(DIK_W) - s_input->PushKey(DIK_S)) * m_addSpeed;
		}
		else
		{
			m_vel.x *= 0.8f;
			m_vel.z *= 0.8f;
			if (fabs(m_vel.x) < 0.0001f)
			{
				m_vel.x = 0;
			}
			if (fabs(m_vel.z) < 0.0001f)
			{
				m_vel.z = 0;
			}
		}
		// ジャンプ
		if (s_input->PullLeftTrigger())
		{
			add.y += 0.75f;

			m_gravityTime = 0;
		}
		else if (s_input->PushKey(DIK_RETURN))
		{
			add.y += 0.75f;

			m_gravityTime = 0;
		}
		// 重力
		add.y += 0 - 4.9f * powf(static_cast<float>(m_gravityTime) / 60, 2);
		m_gravityTime++;

		// ベクトルに加算
		m_vel.x += add.x;
		m_vel.y += add.y;
		m_vel.z += add.z;
		if (m_maxSpeed < fabs(m_vel.x))
		{
			m_vel.x = m_maxSpeed * (m_vel.x / fabs(m_vel.x));
		}
		if (1.0f < m_vel.y)
		{
			m_vel.y = 1.0f;
		}
		else if (m_vel.y < -9.8f)
		{
			m_vel.y = -9.8f;
		}
		if (m_maxSpeed < fabs(m_vel.z))
		{
			m_vel.z = m_maxSpeed * (m_vel.z / fabs(m_vel.z));
		}

		// ダッシュ
		if (s_input->SwitchRightTrigger() && m_isDash == false)
		{
			m_isDash = true;
			m_dashSpeed = 4.0f;
		}
		else if (s_input->TriggerKey(DIK_SPACE) && m_isDash == false)
		{
			m_isDash = true;
			m_dashSpeed = 4.0f;
		}
		if (m_isDash == true)
		{
			m_vel.x *= m_dashSpeed;
			m_vel.z *= m_dashSpeed;

			m_dashSpeed -= 0.3f;
			if (m_dashSpeed < 1.0f)
			{
				m_dashSpeed = 0;
				m_isDash = false;
			}
		}

		// カメラを軸にした変換
		m_pos = s_camera->ConvertWindowPos(m_object->GetPosition(), m_vel);
		
		// 壁との当たり判定
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
		if (m_pos.y < -100 - 1.875f)
		{
			m_pos.y = -100 + 1.875f;

			m_gravityTime = 0;
		}

		// 座標セット
		m_object->SetPosition(m_pos);
	}
}

void Player::Draw()
{
	if (m_alive == true)
	{
		m_object->Draw();
	}
}

const float Player::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
