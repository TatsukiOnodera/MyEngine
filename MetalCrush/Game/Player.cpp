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

	// ステータス
	m_status.isAlive = true;
	m_status.isDash = false;
	m_status.HP = 20;
	m_status.pos = { 0, 0, -100 };
	m_status.vel = { 0, 0, 0 };

	// ダッシュの加速度
	m_dashAcc = 0;
	// ダッシュの時間
	m_dashTime = 0;
	// 重力時間
	m_gravityTime = 0;

	// 標的の座標
	m_targetPos = { 0, 0, 0 };
	// ロック中か
	m_isLock = false;
	// 発射間隔
	m_shotInterval = 0;
	// 装弾数
	m_bulletCapacity = 20;
	// リロードタイム
	m_reloadTimer = 0;

	// カメラ位置の正面化
	m_cameraInitialize = false;

	// オブジェクト
	m_object->SetPosition(m_status.pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	// 生きているなら
	if (m_status.isAlive == true)
	{
		// 重力時間
		m_gravityTime++;

		// 加速度
		XMFLOAT3 acc = {};

		// 縦横移動
		MovePlayer(acc);

		// 上下の移動
		JumpPlayer(acc);

		// 速度に加速を加算
		AddAcceleration(acc);

		// ダッシュ
		DashPlayer();

		// カメラを軸にした変換
		m_status.pos = s_camera->ConvertWindowYPos(m_status.pos, m_status.vel);

		// 座標セット
		m_object->SetPosition(m_status.pos);

		// ショット
		ShotBullet();
	}

	// 弾の更新
	for (auto& m : playerBullets)
	{
		m->Update();
	}

	// カメラワーク
	CameraWork();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);
	// FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	if (m_status.isAlive == true)
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
		// 加速
		acc.x += s_input->LeftStickAngle().x * c_accMove;
		acc.z += s_input->LeftStickAngle().y * c_accMove;
	}
	else
	{
		// 減速
		m_status.vel.x *= c_decMove;
		m_status.vel.z *= c_decMove;

		// 一定以下なら0に
		if (fabs(m_status.vel.x) < 0.001f)
		{
			m_status.vel.x = 0;
		}
		if (fabs(m_status.vel.z) < 0.001f)
		{
			m_status.vel.z = 0;
		}
	}
}

void Player::JumpPlayer(XMFLOAT3& acc)
{
	// ジャンプ
	if (s_input->PullLeftTrigger())
	{
		acc.y += c_accJump;
		m_gravityTime = 0;
	}

	// 重力
	acc.y += -0.98f * powf(static_cast<float>(m_gravityTime) / 60, 2);
}

void Player::AddAcceleration(const XMFLOAT3& acc)
{
	m_status.vel.x += acc.x;
	m_status.vel.y += acc.y;
	m_status.vel.z += acc.z;

	// 速度を調整
	// X軸
	if (c_maxVelXZ < fabs(m_status.vel.x))
	{
		float tmp_div = fabs(m_status.vel.x) / c_maxVelXZ;
		m_status.vel.z /= tmp_div;
		m_status.vel.x /= tmp_div;
	}
	// Z軸
	if (c_maxVelXZ < fabs(m_status.vel.z))
	{
		float tmp_div = fabs(m_status.vel.z) / c_maxVelXZ;
		m_status.vel.x /= tmp_div;
		m_status.vel.z /= tmp_div;
	}
	// Y軸
	if (c_maxVelY < m_status.vel.y)
	{
		m_status.vel.y = c_maxVelY;
	}
}

void Player::DashPlayer()
{
	// ダッシュ
	if(s_input->SwitchRightTrigger() && m_status.isDash == false)
	{
		m_status.isDash = true;
	}

	// 加速
	if (m_status.isDash == true)
	{
		m_dashTime += m_addDT;

		if (60 <= m_dashTime)
		{
			m_dashAcc = 60;
			m_addDT = -fabs(m_addDT);
		}
		else if (m_dashTime <= 0)
		{
			m_dashTime = 0;
			m_addDT = fabs(m_addDT);
			m_status.isDash = false;
		}

		float tmp_t = 0;
		if (0 < fabs(m_addDT))
		{
			tmp_t = (m_dashTime / 60) * (2 - (m_dashTime) / 60);
		}
		else
		{
			tmp_t = (m_dashTime / 60) * (m_dashTime / 60);
		}

		m_dashAcc = 4.0f * tmp_t;
	}

	m_status.vel.x *= 1 + m_dashAcc;
	m_status.vel.z *= 1 + m_dashAcc;
}

void Player::ShotBullet()
{
	m_shotInterval++;

	if (0 < m_bulletCapacity && m_reloadTimer == 0)
	{
		// 発射
		if (s_input->PushButton(BUTTON::RB) && 8 < m_shotInterval)
		{
			m_shotInterval = 0;

			XMFLOAT3 vel = {};
			if (m_isLock == true)
			{
				// 標的までの方向を取得
				vel.x = m_targetPos.x - m_status.pos.x;
				vel.y = m_targetPos.y - m_status.pos.y;
				vel.z = m_targetPos.z - m_status.pos.z;

				// 長さを正規化して10倍する
				float len = sqrtf(powf(vel.x, 2) + powf(vel.y, 2) + powf(vel.z, 2));
				vel.x = vel.x / len * 6;
				vel.y = vel.y / len * 6;
				vel.z = vel.z / len * 6;
			}
			else
			{
				vel = { 0, 0, 6 };

				// カメラの方向に変換
				vel = s_camera->ConvertWindowXYPos({ 0, 0, 0 }, vel);
			}

			// 使っていない弾を探す
			if (CheckNoUsingBullet() == true)
			{
				for (auto& m : playerBullets)
				{
					if (m->GetAlive() == false)
					{
						m->Initialize(m_status.pos, vel, true);
						m_bulletCapacity--;
						break;
					}
				}
			}
			else
			{
				playerBullets.emplace_back(new Bullet(m_status.pos, vel, true));
				m_bulletCapacity--;
			}
		}
	}
	else
	{
		// リロードs
		m_reloadTimer++;
		if (m_reloadTimer % 6 == 0)
		{
			m_bulletCapacity++;
		}
		if (20 * 6 <= m_reloadTimer)
		{
			m_reloadTimer = 0;
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

void Player::CameraWork()
{
	// 遅れて追従
	XMFLOAT3 tPos = m_status.pos;
	XMFLOAT3 tVel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_status.vel);
	// 速度分引く
	tPos.x -= tVel.x;
	tPos.y -= tVel.y;
	tPos.z -= tVel.z;

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
			cameraAngle.y += s_input->RightStickAngle().x * c_addAngle;
			cameraAngle.x -= s_input->RightStickAngle().y * c_addAngle;
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
	m_gravityTime = 0;
	m_status.vel.y = 0;
}
