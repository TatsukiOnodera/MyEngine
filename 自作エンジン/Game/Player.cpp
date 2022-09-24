#include "Player.h"
#include "DirectXCommon.h"

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

	m_pos = { 0, 0, -50 };
	m_add = { 0, 0, 0 };
	m_speed = 1.0f;
	m_alive = true;
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
		// 移動ベクトル
		XMFLOAT3 vec = {};
		// 移動
		if (s_input->PushKey(DIK_D) || s_input->PushKey(DIK_A) || s_input->PushKey(DIK_W) || s_input->PushKey(DIK_S))
		{
			vec.x += (s_input->PushKey(DIK_D) - s_input->PushKey(DIK_A)) * m_speed;
			vec.z += (s_input->PushKey(DIK_W) - s_input->PushKey(DIK_S)) * m_speed;
		}
		// ジャンプ
		if (s_input->PushKey(DIK_C))
		{
			vec.y += 1.0f;
		}
		vec.y -= 0.98f;

		// カメラを軸にした変換
		m_pos = s_camera->ConvertWindowPos(m_object->GetPosition(), vec);
		
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
		if (m_pos.y - 4.0f < -100)
		{
			m_pos.y = -100 + 4.0f;
		}
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
