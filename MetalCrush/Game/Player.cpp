#include "DirectXCommon.h"

#include "Player.h"

#include <time.h>

Camera* Player::s_camera = Camera::GetInstance();
Input* Player::s_input = Input::GetInstance();

using namespace DirectX;

Player::Player()
{
	// モデルの読み込み
	m_playerFBX.reset(FbxObject3d::CreateFBXObject("PlayerBone"));
	m_booster.reset(ParticleManager::Create("Particle/FireParticle.png"));

	// 初期化
	Initialize();
}

Player::~Player()
{
	
}

void Player::Initialize()
{
	// nullチェック
	if (m_playerFBX == nullptr)
	{
		assert(0);
	}

	// ステータス
	m_player.isAlive = true;
	m_player.isDash = false;
	m_player.HP = 20;
	m_player.pos = { 0, 0, -100 };
	m_player.vel = { 0, 0, 0 };

	// ダッシュの加速度
	m_dashAcc = 0;
	// ダッシュの時間
	m_dashTime = 0;
	// 1フレーム当たりの加算（DT = DashTime）
	m_addDT = 8.0f;

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

	for (int i = 0; i < 2; i++)
	{
		m_mBoosterPos[i] =XMFLOAT3(0, 0, 0);
		m_sBoosterPos[i] = XMFLOAT3(0, 0, 0);
		m_bBoosterPos[i] = XMFLOAT3(0, 0, 0);
	}

	// オブジェクト
	m_playerFBX->SetPosition(m_player.pos);
	m_playerFBX->SetScale({ 0.25f, 0.25f, 0.25f });
	m_playerFBX->SetRotation({ -90, 180, 0 });
	m_playerFBX->Update();
}

void Player::Update()
{
	// 生きているなら
	if (m_player.isAlive == true)
	{
		// 加速度
		XMFLOAT3 acc = {};

		// 縦横移動
		MovePlayer(acc);

		// 上下の移動
		JumpPlayer(acc);

		// 重力
		acc.y += (-0.98f * 0.5f) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// 重力時間
		m_gravityTime++;

		// 速度に加速を加算
		AddAcceleration(acc);

		// ダッシュ
		DashPlayer();

		// カメラを軸にした変換
		m_player.pos = s_camera->ConvertWindowYPos(m_player.pos, m_player.vel);

		// 座標セット
		m_playerFBX->SetPosition(m_player.pos);

		// ショット
		ShotBullet();
	}

	// 弾の更新
	for (auto& m : m_playerBullets)
	{
		m->Update();
	}

	// メインブースターのエフェクト
	MainBooster();

	// サイドブースターのエフェクト
	SideBooster();

	// バックブースターのエフェクト
	BacktBooster();

	// カメラワーク
	CameraWork();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);
	
	if (0 < m_playerBullets.size())
	{
		for (auto& m : m_playerBullets)
		{
			m->Draw();
		}
	}
	
	Object3d::PostDraw();

	// FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	if (m_player.isAlive == true)
	{
		// 自機
		m_playerFBX->Draw();
	}

	FbxObject3d::PostDraw();

	// パーティクル描画
	// ブースター
	m_booster->Draw(cmdList);
}

void Player::MovePlayer(XMFLOAT3& acc)
{
	if (s_input->LeftStickAngle().x != 0 || s_input->LeftStickAngle().y != 0)
	{
		// 加速
		acc.x += s_input->LeftStickAngle().x * c_accMove;
		acc.z += s_input->LeftStickAngle().y * c_accMove;
	}
	else if (m_player.isDash == false)
	{
		// 減速
		m_player.vel.x *= c_decMove;
		m_player.vel.z *= c_decMove;

		// 一定以下なら0に
		if (fabs(m_player.vel.x) < 0.001f)
		{
			m_player.vel.x = 0;
		}
		if (fabs(m_player.vel.z) < 0.001f)
		{
			m_player.vel.z = 0;
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
}

void Player::AddAcceleration(const XMFLOAT3& acc)
{
	m_player.vel.x += acc.x;
	m_player.vel.y += acc.y;
	m_player.vel.z += acc.z;

	// 速度を調整
	// X軸
	if (c_maxVelXZ < fabs(m_player.vel.x))
	{
		float tmp_div = fabs(m_player.vel.x) / c_maxVelXZ;
		m_player.vel.z /= tmp_div;
		m_player.vel.x /= tmp_div;
	}
	// Z軸
	if (c_maxVelXZ < fabs(m_player.vel.z))
	{
		float tmp_div = fabs(m_player.vel.z) / c_maxVelXZ;
		m_player.vel.x /= tmp_div;
		m_player.vel.z /= tmp_div;
	}
	// Y軸
	if (c_maxVelY < m_player.vel.y)
	{
		m_player.vel.y = c_maxVelY;
	}
}

void Player::DashPlayer()
{
	// ダッシュ
	if(s_input->SwitchRightTrigger() && m_player.isDash == false && m_player.vel.x != 0 && m_player.vel.z != 0)
	{
		m_player.isDash = true;
	}

	// 加速
	if (m_player.isDash == true)
	{
		m_dashTime += m_addDT;

		if (60 <= m_dashTime)
		{
			m_dashTime = 60;
			m_addDT = -3.0f;
		}
		else if (m_dashTime <= 0)
		{
			m_dashTime = 0;
			m_addDT = 8.0f;
			m_player.isDash = false;
		}

		float tmp_t = 0;
		if (0 < fabs(m_addDT))
		{
			tmp_t = (m_dashTime / 60) * (2 - (m_dashTime) / 60);
		}

		m_dashAcc = 4.0f * tmp_t;

		m_player.vel.x *= 1 + m_dashAcc;
		m_player.vel.z *= 1 + m_dashAcc;
	}
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
				vel.x = m_targetPos.x - m_player.pos.x;
				vel.y = m_targetPos.y - m_player.pos.y;
				vel.z = m_targetPos.z - m_player.pos.z;

				// 長さを正規化して10倍する
				float len = sqrtf(powf(vel.x, 2) + powf(vel.y, 2) + powf(vel.z, 2));
				vel.x = vel.x / len * 5;
				vel.y = vel.y / len * 5;
				vel.z = vel.z / len * 5;
			}
			else
			{
				vel = { 0, 0, 5 };

				// カメラの方向に変換
				vel = s_camera->ConvertWindowXYPos({ 0, 0, 0 }, vel);
			}

			// 使っていない弾を探す
			if (CheckNoUsingBullet() == true)
			{
				for (auto& m : m_playerBullets)
				{
					if (m->GetAlive() == false)
					{
						m->Initialize(m_player.pos, vel, XMFLOAT3(0, 0, 0), true);
						m_bulletCapacity--;
						break;
					}
				}
			}
			else
			{
				m_playerBullets.emplace_back(new Bullet(m_player.pos, vel));
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
	if (0 < m_playerBullets.size())
	{
		// 使っていないのがあるか
		bool hit = false;
		for (const auto& m : m_playerBullets)
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
	XMFLOAT3 tPos = m_player.pos;
	XMFLOAT3 tVel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_player.vel);
	// 速度分引く
	tPos.x -= tVel.x;
	tPos.y -= tVel.y;
	tPos.z -= tVel.z;

	// 追従カメラ
	XMFLOAT3 cameraAngle = {};
	if (s_input->RightStickAngle().x != 0 || s_input->RightStickAngle().y != 0)
	{
		cameraAngle.y += s_input->RightStickAngle().x * c_addAngle;
		cameraAngle.x -= s_input->RightStickAngle().y * c_addAngle;
	}
	s_camera->FollowUpCamera(tPos, s_camera->GetDistance(), cameraAngle);

	// 姿勢制御
	XMFLOAT3 tmp_rot = m_playerFBX->GetRotation();
	tmp_rot.y += cameraAngle.y;
	m_playerFBX->SetRotation(tmp_rot);
}

void Player::OnLand()
{
	m_gravityTime = 0;
	m_player.vel.y = 0;
}

void Player::MainBooster()
{
	// オフセット
	XMFLOAT3 posA = { 1.0f * m_playerFBX->GetScale().x, 2.5f * m_playerFBX->GetScale().z, -1.5f * m_playerFBX->GetScale().y };
	XMFLOAT3 posB = { -1.0f * m_playerFBX->GetScale().x, 2.5f * m_playerFBX->GetScale().z, -1.5f * m_playerFBX->GetScale().y };
	posA = s_camera->ConvertWindowYPos(m_player.pos, posA);
	posB = s_camera->ConvertWindowYPos(m_player.pos, posB);
	m_mBoosterPos[0] = posA;
	m_mBoosterPos[1] = posB;

	if (0 < m_player.vel.z || 0 < m_player.vel.y)
	{
		// ベクトル
		XMFLOAT3 tmp_vel = {};
		if (0 < m_player.vel.y)
		{
			tmp_vel.y = -normalize(m_player.vel).y;
		}
		if (0 < m_player.vel.z)
		{
			tmp_vel.z = -normalize(m_player.vel).z;
		}
		tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

		if (m_player.isDash == false)
		{
			tmp_vel.y *= 0.01f;
			tmp_vel.z *= 0.01f;

			for (int i = 0; i < 2; i++)
			{
				m_booster->Add(5, posA, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.1f, true, &m_mBoosterPos[0]);
				m_booster->Add(5, posB, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.1f, true, &m_mBoosterPos[1]);
			}
		}
		else
		{
			tmp_vel.y = 0;
			tmp_vel.z *= 1.0f;

			for (int i = 0; i < 3; i++)
			{
				m_booster->Add(10, posA, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.5f, 1.5f, true, &m_mBoosterPos[0]);
				m_booster->Add(10, posB, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.5f, 1.5f, true, &m_mBoosterPos[1]);
			}
		}
	}
}

void Player::SideBooster()
{
	// オフセット
	XMFLOAT3 posA = { -2.5f * m_playerFBX->GetScale().x, 3.5f * m_playerFBX->GetScale().z, 0 };
	posA = s_camera->ConvertWindowYPos(m_player.pos, posA);
	m_sBoosterPos[0] = posA;
	XMFLOAT3 posB = { 2.5f * m_playerFBX->GetScale().x, 3.5f * m_playerFBX->GetScale().z, 0 };
	posB = s_camera->ConvertWindowYPos(m_player.pos, posB);
	m_sBoosterPos[1] = posB;

	if (m_player.vel.x != 0)
	{
		// ベクトル
		XMFLOAT3 tmp_vel = {};
		tmp_vel.x = -normalize(m_player.vel).x;
		tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

		if (m_player.isDash == false)
		{
			tmp_vel.x *= 0.01f;

			if (0 < m_player.vel.x)
			{
				for (int i = 0; i < 3; i++)
				{
					m_booster->Add(5, posA, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.1f, true, &m_sBoosterPos[0]);
				}
			}
			else if (m_player.vel.z < 0)
			{
				for (int i = 0; i < 3; i++)
				{
					m_booster->Add(5, posB, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.1f, true, &m_sBoosterPos[1]);
				}
			}
		}
		else
		{
			tmp_vel.x *= 0.5f;

			if (0 < m_player.vel.x)
			{
				for (int i = 0; i < 3; i++)
				{
					m_booster->Add(10, posA, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.5f, 1.5f, true, &m_sBoosterPos[0]);
				}
			}
			else if (m_player.vel.x < 0)
			{
				for (int i = 0; i < 3; i++)
				{
					m_booster->Add(10, posB, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.5f, 1.5f, true, &m_sBoosterPos[1]);
				}
			}
		}
	}
}

void Player::BacktBooster()
{
	// オフセット
	XMFLOAT3 posA = { -1.0f * m_playerFBX->GetScale().x, 0.5f * m_playerFBX->GetScale().z, 0.5f * m_playerFBX->GetScale().y };
	XMFLOAT3 posB = { 1.0f * m_playerFBX->GetScale().x, 0.5f * m_playerFBX->GetScale().z, 0.5f * m_playerFBX->GetScale().y };
	posA = s_camera->ConvertWindowYPos(m_player.pos, posA);
	posB = s_camera->ConvertWindowYPos(m_player.pos, posB);
	m_bBoosterPos[0] = posA;
	m_bBoosterPos[1] = posB;

	if (m_player.vel.z < 0)
	{
		XMFLOAT3 tmp_vel = {};
		tmp_vel.z = -normalize(m_player.vel).z;
		tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

		if (m_player.isDash == false)
		{
			tmp_vel.z *= 0.01f;

			for (int i = 0; i < 3; i++)
			{
				m_booster->Add(5, posA, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.1f, true, &m_bBoosterPos[0]);
				m_booster->Add(5, posB, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.1f, true, &m_bBoosterPos[1]);
			}
		}
		else
		{
			tmp_vel.z *= 0.5f;

			for (int i = 0; i < 3; i++)
			{
				m_booster->Add(10, posA, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.5f, 1.5f, true, &m_bBoosterPos[0]);
				m_booster->Add(10, posB, tmp_vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.2f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.5f, 1.5f, true, &m_bBoosterPos[1]);
			}
		}
	}
}

const float Player::Length(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

const XMFLOAT3 Player::normalize(const XMFLOAT3& tmp)
{
	float len = Length(tmp);

	XMFLOAT3 _normalize = { tmp.x / len, tmp.y / len, tmp.z / len };

	return _normalize;
}
