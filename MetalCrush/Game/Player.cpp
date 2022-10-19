#include "DirectXCommon.h"

#include "Player.h"

Camera* Player::s_camera = Camera::GetInstance();
Input* Player::s_input = Input::GetInstance();

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
	// nullチェック
	if (m_object == nullptr)
	{
		assert(0);
	}

	// 生死
	m_alive = true;
	// 座標
	m_pos = { 0, 0, -100 };
	// 速度
	m_vel = { 0, 0, 0 };
	// 毎加速度
	m_accSpeed = 0.1f;
	// 減速度
	m_decSpeed = 0.8f;
	// ジャンプ加速度
	m_accJump = 0.1f;
	// 重力時間
	m_gravityTime = 0;
	// ダッシュフラグ
	m_isDash = false;
	// ダッシュ加速比
	m_dashTimes = 1.0f;
	// ダッシュ加速比の毎加算値
	m_accDashTimes = 0.6f;
	// カメラの回転角度
	m_addAngle = 1.0f;
	// カメラ位置の正面化
	m_cameraInitialize = false;

	// 標的の座標
	m_targetPos = { 0, 0, 0 };
	// ロック中か
	m_isLock = false;
	// 発射間隔
	m_shotInterval = 0;

	// オブジェクト
	m_object->SetPosition(m_pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	// 加速度
	XMFLOAT3 acc = {};

	// 生きているなら
	if (m_alive == true)
	{
		// 重力時間
		m_gravityTime++;

		// 移動
		MovePlayer(acc);

		// ジャンプ
		JumpPlayer(acc);

		// 重力
		acc.y -= (9.8f / 10) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// 速度に加速を加算
		AddAcceleration(acc);

		// ダッシュ
		DashPlayer();

		// カメラを軸にした変換
		m_pos = s_camera->ConvertWindowYPos(m_pos, m_vel);

		// 座標セット
		m_object->SetPosition(m_pos);

		// ショット
		ShotBullet();
	}

	// 弾の更新
	for (auto& m : playerBullets)
	{
		m->Update();
	}

	// カメラワーク
	MoveCamera();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);
	// FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	if (m_alive == true)
	{
		// 自機
		m_object->Draw();
	}
	if (0 < playerBullets.size())
	{
		for (auto& m : playerBullets)
		{
			m->Draw();
		}
	}

	Object3d::PostDraw();
	FbxObject3d::PostDraw();
}

void Player::MovePlayer(XMFLOAT3& acc)
{
	if (s_input->LeftStickAngle().x != 0 || s_input->LeftStickAngle().y != 0)
	{
		acc.x += s_input->LeftStickAngle().x * m_accSpeed;
		acc.z += s_input->LeftStickAngle().y * m_accSpeed;
	}
	else
	{
		m_vel.x *= m_decSpeed;
		m_vel.z *= m_decSpeed;
	}
}

void Player::JumpPlayer(XMFLOAT3& acc)
{
	if (s_input->PullLeftTrigger())
	{
		acc.y += m_accJump;
		m_gravityTime = 0;
	}
}

void Player::AddAcceleration(const XMFLOAT3& acc)
{
	m_vel.x += acc.x;
	m_vel.y += acc.y;
	m_vel.z += acc.z;

	if (1.0f < fabs(m_vel.x))
	{
		m_vel.z /= fabs(m_vel.x);
		m_vel.x /= fabs(m_vel.x);
	}
	if (1.0f < fabs(m_vel.z))
	{
		m_vel.x /= fabs(m_vel.z);
		m_vel.z /= fabs(m_vel.z);
	}
	if (1.0f < m_vel.y)
	{
		m_vel.y = 1.0f;
	}
}

void Player::DashPlayer()
{
	if (m_isDash == false)
	{
		if (s_input->SwitchRightTrigger())
		{
			m_isDash = true;
		}
	}
	else
	{
		// 加速
		m_vel.x *= m_dashTimes;
		m_vel.z *= m_dashTimes;

		// 加速比に加算
		m_dashTimes += m_accDashTimes;

		// 加速値が5より大きいなら
		if (5.0f < m_dashTimes)
		{
			m_dashTimes = 5.0f;
			m_accDashTimes = -0.2f;
		}
		// 加速値が1を下回ったなら
		else if (m_dashTimes < 1.0f)
		{
			m_dashTimes = 1.0f;
			m_accDashTimes = 0.6f;
			m_isDash = false;
		}
	}
}

void Player::ShotBullet()
{
	m_shotInterval++;

	if (s_input->PushButton(BUTTON::RB) && 8 < m_shotInterval)
	{
		m_shotInterval = 0;

		XMFLOAT3 vel = {};
		if (m_isLock == true)
		{
			// 標的の座標を取得
			vel.x = m_targetPos.x - m_pos.x;
			vel.y = m_targetPos.y - m_pos.y;
			vel.z = m_targetPos.z - m_pos.z;

			// 長さを1にして10倍する
			float len = sqrtf(powf(vel.x, 2) + powf(vel.y, 2) + powf(vel.z, 2));
			vel.x = vel.x / len * 10;
			vel.y = vel.y / len * 10;
			vel.z = vel.z / len * 10;
		}
		else
		{
			vel = { 0, 0, 10 };
			vel = s_camera->ConvertWindowXYPos({ 0, 0, 0 }, vel);
		}
		if (CheckNoUsingBullet() == true)
		{
			for (auto& m : playerBullets)
			{
				if (m->GetAlive() == false)
				{
					m->Initialize(m_pos, vel, true);
					break;
				}
			}
		}
		else
		{
			playerBullets.emplace_back(new Bullet(m_pos, vel, true));
		}
	}
}

bool Player::CheckNoUsingBullet()
{
	if (0 < playerBullets.size())
	{
		// 使っていないのがあるか
		bool hit = false;
		for (const auto& m : playerBullets)
		{
			if (m->GetAlive() == false)
			{
				hit = true;
				break;
			}
		}

		return hit;
	}
	else
	{
		return false;
	}
}

void Player::MoveCamera()
{
	// 遅れて追従
	XMFLOAT3 tPos = m_pos;
	XMFLOAT3 tVel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_vel);
	tPos.x -= tVel.x;
	tPos.y -= tVel.y;
	if (0 <= m_vel.z)
	{
		tPos.z -= tVel.z;
	}
	else
	{
		tPos.z -= tVel.z * 0.3f;
	}

	// 追従カメラ
	XMFLOAT3 cameraAngle = {};
	if (m_cameraInitialize == false)
	{
		if (s_input->PushButton(BUTTON::R_STICK))
		{
			m_cameraInitialize = true;
		}
		else if (s_input->RightStickAngle().x != 0 || s_input->RightStickAngle().y != 0)
		{
			cameraAngle.y += s_input->RightStickAngle().x * m_addAngle;
			cameraAngle.x -= s_input->RightStickAngle().y * m_addAngle;
		}
	}
	else
	{
		// カメラ正面化
		if (true)
		{
			m_cameraInitialize = false;
		}
	}

	s_camera->FollowUpCamera(tPos, s_camera->GetDistance(), cameraAngle);
}

void Player::OnLand()
{
	m_vel.y = 0;
	m_gravityTime = 0;
}
