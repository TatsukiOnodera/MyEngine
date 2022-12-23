#include "DirectXCommon.h"
#include "Model.h"
#include "Player.h"
#include "SphereCollider.h"
#include <Tool.h>

Camera* Player::s_camera = Camera::GetInstance();
Input* Player::s_input = Input::GetInstance();

using namespace DirectX;

Player::Player()
{
	// モデルの読み込み
	m_playerLeg.reset(Object3d::Create(4));
	m_playerBody.reset(Object3d::Create(5));
	m_playerArm.reset(Object3d::Create(6));
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
	if (m_playerLeg == nullptr || m_booster == nullptr)
	{
		assert(0);
	}

	// プレイヤー
	m_player.isAlive = true;
	m_player.isDash = false;
	m_player.boostGauge = 360;
	m_player.HP = 20;
	m_player.pos = { 0, 0, -100 };
	m_player.vel = { 0, 0, 0 };

	// 通常移動
	m_move.acc = 0.0325f;
	m_move.dec = 0.7f;
	m_move.maxVelXZ = 0.8f;

	// ジャンぴ
	m_jump.acc = 0.025f;
	m_jump.maxVelY = 0.8f;

	// ダッシュ
	m_dash.acc = 0;
	m_dash.time = 0;
	m_dash.DT = 8.0f;

	// 重力時間
	m_gravityTime = 0;

	// バレット
	m_bullet.targetPos = { 0, 0, 0 };
	m_bullet.num = m_bullet.maxNum;
	m_bullet.speed = 7;
	m_bullet.interval = 0;
	m_bullet.reloadTimer = 0;
	m_bullet.isReload = false;
	m_bullet.isLock = false;

	// ブースター
	m_gauge.coolTime = 0;
	m_gauge.add = 2;
	m_gauge.dashDec = 130;
	m_gauge.jumpDec = m_gauge.add + 1;
	m_gauge.max = m_player.boostGauge;

	// ブースターオフセット
	for (int i = 0; i < 2; i++)
	{
		m_offset.main[i] =XMFLOAT3(0, 0, 0);
		m_offset.main[i] = XMFLOAT3(0, 0, 0);
		m_offset.back[i] = XMFLOAT3(0, 0, 0);
	}
	XMFLOAT3 mOffset = { 0.1f, 0.15f, -0.55f };
	m_offset.mainOffset[0] = { -mOffset.x * m_playerLeg->GetScale().x, mOffset.y * m_playerLeg->GetScale().y, mOffset.z * m_playerLeg->GetScale().z};
	m_offset.mainOffset[1] = { mOffset.x * m_playerLeg->GetScale().x, mOffset.y * m_playerLeg->GetScale().y, mOffset.z * m_playerLeg->GetScale().z };
	XMFLOAT3 sOffset = { 0.65f, 0.55f, 0 };
	m_offset.sideOffset[0] = { sOffset.x * m_playerLeg->GetScale().x, sOffset.y * m_playerLeg->GetScale().y, sOffset.z * m_playerLeg->GetScale().z };
	m_offset.sideOffset[1] = { -sOffset.x * m_playerLeg->GetScale().x, sOffset.y * m_playerLeg->GetScale().y, sOffset.z * m_playerLeg->GetScale().z };
	XMFLOAT3 bOffset = { 0.25f, 0, 0.25f };
	m_offset.backOffset[0] = { -bOffset.x * m_playerLeg->GetScale().x, bOffset.y * m_playerLeg->GetScale().z, bOffset.z * m_playerLeg->GetScale().y };
	m_offset.backOffset[1] = { bOffset.x * m_playerLeg->GetScale().x, bOffset.y * m_playerLeg->GetScale().z, bOffset.z * m_playerLeg->GetScale().y };

	// カメラ
	m_camera.isMove = true;
	m_camera.addAngle = 1.0f;

	// オブジェクト
	m_playerLeg->SetPosition(m_player.pos);
	m_playerLeg->SetScale({ 0.25f, 0.25f, 0.25f });
	m_playerLeg->SetRotation({ 0, -90, 0 });
	m_playerLeg->SetCollider(new SphereCollider({ 0, 0, 0 }, 3));
	m_playerLeg->Update();
	m_playerBody->SetParent(m_playerLeg.get());
	m_playerBody->SetPosition({ 0, 1.5f, 0 });
	m_playerBody->Update();
	m_playerArm->SetParent(m_playerBody.get());
	m_playerArm->SetPosition({ 0, 0.75f, 0 });
	m_playerArm->SetRotation({ 0, 0, 90 });
	m_playerArm->Update();
}

void Player::Update()
{
	// 加速度
	XMFLOAT3 acc = {};	

	// 生きているなら
	if (m_player.isAlive == true)
	{
		// 縦横移動
		MovePlayer(acc);

		// 上下の移動
		JumpPlayer(acc);

		// 重力
		acc.y += (-0.98f * 0.25f) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// 重力時間
		m_gravityTime++;

		// 速度に加速を加算
		NormalizeVel(acc);

		// ダッシュ
		DashPlayer();

		// カメラを軸にした変換
		m_player.pos = s_camera->ConvertWindowYPos(m_player.pos, m_player.vel);

		// 座標セット
		m_playerLeg->SetPosition(m_player.pos);

		// ショット
		ShotBullet();

		// オフセットの更新
		UpdateOffset();

		if (m_player.isDash == false)
		{
			// メインブースターのパーティクル
			MainBooster();

			// サイドブースターのパーティクル
			SideBooster();

			// バックブースターのパーティクル
			BacktBooster();
		}
		else
		{
			// ダッシュ時のパーティクル
			DashBooster(acc);
		}

		// ブーストゲージ
		BoosterGauge();
	}

	// 弾の更新
	for (const auto& m : m_playerBullets)
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
	
	for (auto& m : m_playerBullets)
	{
		m->Draw();
	}
	if (m_player.isAlive == true)
	{
		m_playerLeg->Draw();
		m_playerBody->Draw();
		m_playerArm->Draw();
	}
	
	Object3d::PostDraw();

	// パーティクル描画
	m_booster->Draw(cmdList);
}

void Player::MovePlayer(XMFLOAT3& acc)
{
	if (s_input->LeftStickAngle().x != 0 || s_input->LeftStickAngle().y != 0)
	{
		// 加速
		acc.x += s_input->LeftStickAngle().x * m_move.acc;
		acc.z += s_input->LeftStickAngle().y * m_move.acc;
	}
	else
	{
		// 減速
		m_player.vel.x *= m_move.dec;
		m_player.vel.z *= m_move.dec;

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
	if (s_input->PullLeftTrigger() && m_gauge.coolTime == 0)
	{
		acc.y += m_jump.acc;
		m_gravityTime = 0;
		/*m_player.boostGauge -= m_gauge.jumpDec;
		if (m_player.boostGauge < 0)
		{
			m_gauge.coolTime = 1;
			m_player.boostGauge = 0;
		}*/
	}
}

void Player::DashPlayer()
{
	// ダッシュ
	if(s_input->SwitchRightTrigger() && m_player.isDash == false && m_player.vel.x != 0 && m_player.vel.z != 0 && m_gauge.coolTime == 0)
	{
		m_player.isDash = true;
		m_player.boostGauge -= m_gauge.dashDec;
		if (m_player.boostGauge < 0)
		{
			m_gauge.coolTime = 1;
			m_player.boostGauge = 0;
		}
	}

	// 加速
	if (m_player.isDash == true)
	{
		m_dash.time += m_dash.DT;

		if (60 <= m_dash.time)
		{
			m_dash.time = 60;
			m_dash.DT = -3.0f;
		}
		else if (m_dash.time <= 0)
		{
			m_dash.time = 0;
			m_dash.DT = 8.0f;
			m_player.isDash = false;
		}

		float tmp_t = 0;
		if (0 < fabs(m_dash.DT))
		{
			tmp_t = (m_dash.time / 60) * (2 - (m_dash.time) / 60);
		}

		m_dash.acc = 5.0f * tmp_t;

		m_player.vel.x *= 1 + m_dash.acc;
		m_player.vel.z *= 1 + m_dash.acc;
	}
}

void Player::NormalizeVel(const XMFLOAT3& acc)
{
	m_player.vel.x += acc.x;
	m_player.vel.y += acc.y;
	m_player.vel.z += acc.z;
	if (m_move.maxVelXZ < fabs(m_player.vel.x))
	{
		float div = fabs(m_player.vel.x) / m_move.maxVelXZ;
		m_player.vel.x /= div;
		m_player.vel.z /= div;
	}
	else if (m_move.maxVelXZ < fabs(m_player.vel.z))
	{
		float div = fabs(m_player.vel.z) / m_move.maxVelXZ;
		m_player.vel.x /= div;
		m_player.vel.z /= div;
	}
	if (m_jump.maxVelY < m_player.vel.y)
	{
		m_player.vel.y = m_jump.maxVelY;
	}
}

void Player::ShotBullet()
{
	// 発射間隔
	m_bullet.interval++;

	// 発射
	if (s_input->PushButton(BUTTON::RB) && 15 < m_bullet.interval && m_bullet.isReload == false)
	{
		m_bullet.interval = 0;

		XMFLOAT3 vel = {};
		if (m_bullet.isLock == true)
		{
			// 標的までの方向を取得
			vel.x = m_bullet.targetPos.x - m_player.pos.x;
			vel.y = m_bullet.targetPos.y - m_player.pos.y;
			vel.z = m_bullet.targetPos.z - m_player.pos.z;

			// 長さを正規化する
			vel = Tool::NormalizeFloat3(vel);
			vel.x *= m_bullet.speed;
			vel.y *= m_bullet.speed;
			vel.z *= m_bullet.speed;
		}
		else
		{
			// まっすぐ飛ばす
			vel = { 0, 0, m_bullet.speed };

			// カメラの方向に変換
			vel = s_camera->ConvertWindowXYPos({ 0, 0, 0 }, vel);
		}

		m_playerBullets.emplace_back(new Bullet(m_player.pos, vel, true));
		m_bullet.num--;

		if (m_bullet.num == 0)
		{
			m_bullet.num = m_bullet.maxNum;
			m_bullet.isReload = true;
		}
	}
	// リロード
	else if (m_bullet.isReload == true)
	{
		m_bullet.reloadTimer++;
		if (180 < m_bullet.reloadTimer)
		{
			m_bullet.reloadTimer = 0;
			m_bullet.isReload = false;
		}
	}
}

void Player::CameraWork()
{
	// 焦点座標
	XMFLOAT3 tPos = m_player.pos;
	if (m_camera.isMove == true)
	{
		XMFLOAT3 tVel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_player.vel);
		// 速度分引く
		tPos.x -= tVel.x;
		tPos.y -= tVel.y;
		tPos.z -= tVel.z;
	}

	// 追従カメラ
	XMFLOAT3 cameraAngle = {};
	if (s_input->RightStickAngle().x != 0 || s_input->RightStickAngle().y != 0)
	{
		cameraAngle.y += s_input->RightStickAngle().x * m_camera.addAngle;
		cameraAngle.x -= s_input->RightStickAngle().y * m_camera.addAngle;
	}
	s_camera->FollowUpCamera(tPos, s_camera->GetDistance(), cameraAngle);

	// 姿勢制御
	XMFLOAT3 tmp_rot = m_playerLeg->GetRotation();
	tmp_rot.y += cameraAngle.y;
	m_playerLeg->SetRotation(tmp_rot);
}

void Player::UpdateOffset()
{
	m_offset.main[0] = s_camera->ConvertWindowYPos(m_player.pos, m_offset.mainOffset[0]);
	m_offset.main[1] = s_camera->ConvertWindowYPos(m_player.pos, m_offset.mainOffset[1]);
	m_offset.side[0] = s_camera->ConvertWindowYPos(m_player.pos, m_offset.sideOffset[0]);
	m_offset.side[1] = s_camera->ConvertWindowYPos(m_player.pos, m_offset.sideOffset[1]);
	m_offset.back[0] = s_camera->ConvertWindowYPos(m_player.pos, m_offset.backOffset[0]);
	m_offset.back[1] = s_camera->ConvertWindowYPos(m_player.pos, m_offset.backOffset[1]);
}

void Player::MainBooster()
{
	if (0 < m_player.vel.y || 0 < m_player.vel.z)
	{
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 tmp_vel = Tool::NormalizeFloat3(m_player.vel);
			tmp_vel.x = static_cast<float>(rand() % 3 - 1) / 100;
			if (0 < tmp_vel.y)
			{
				tmp_vel.y = -fabs(tmp_vel.y) * 0.1f + static_cast<float>(rand() % 3 - 1) / 100;
			}
			else
			{
				tmp_vel.y = static_cast<float>(rand() % 3 - 1) / 100;
			}
			if (0 < tmp_vel.z)
			{
				tmp_vel.z = -fabs(tmp_vel.z) * 0.1f + static_cast<float>(rand() % 3 - 1) / 100;
			}
			else
			{
				tmp_vel.z = static_cast<float>(rand() % 3 - 1) / 100;
			}
			tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

			m_booster->Add(5, m_offset.main[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.15f, true, &m_offset.main[0]);
			m_booster->Add(5, m_offset.main[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.15f, true, &m_offset.main[1]);
		}
	}
}

void Player::SideBooster()
{
	if (0.2f < fabs(m_player.vel.x))
	{
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 tmp_vel = Tool::NormalizeFloat3(m_player.vel);
			tmp_vel.x = -(tmp_vel.x * 0.1f) + static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel.y = static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel.z = static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

			if (m_player.vel.x < 0)
			{
				m_booster->Add(5, m_offset.side[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.15f, true, &m_offset.side[0]);
			}
			else
			{
				m_booster->Add(5, m_offset.side[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.15f, true, &m_offset.side[1]);
			}
		}
	}
}

void Player::BacktBooster()
{
	if (m_player.vel.z < 0)
	{
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 tmp_vel = Tool::NormalizeFloat3(m_player.vel);
			tmp_vel.x = static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel.y = static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel.z = fabs(tmp_vel.z) * 0.1f + static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

			m_booster->Add(5, m_offset.back[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.15f, true, &m_offset.back[0]);
			m_booster->Add(5, m_offset.back[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.15f, true, &m_offset.back[1]);
		}
	}
}

void Player::DashBooster(const XMFLOAT3& acc)
{
	if (acc.x != 0 || acc.z != 0)
	{
		if (fabs(acc.x) < fabs(acc.z))
		{
			if (0 < acc.z)
			{
				for (int i = 0; i < 10; i++)
				{
					XMFLOAT3 tmp_vel = Tool::NormalizeFloat3(m_player.vel);
					tmp_vel.x = static_cast<float>(rand() % 21 - 10) / 50;
					tmp_vel.y = static_cast<float>(rand() % 21 - 10) / 50;
					tmp_vel.z = -fabs(tmp_vel.z) * 0.4f + static_cast<float>(rand() % 21 - 10) / 50;
					tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

					m_booster->Add(5, m_offset.main[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.main[0]);
					m_booster->Add(5, m_offset.main[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.main[1]);
				}
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					XMFLOAT3 tmp_vel = Tool::NormalizeFloat3(m_player.vel);
					tmp_vel.x = static_cast<float>(rand() % 21 - 10) / 50;
					tmp_vel.y = static_cast<float>(rand() % 21 - 10) / 50;
					tmp_vel.z = fabs(tmp_vel.z) * 0.4f + static_cast<float>(rand() % 21 - 10) / 50;
					tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

					m_booster->Add(5, m_offset.back[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.back[0]);
					m_booster->Add(5, m_offset.back[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.back[1]);
				}
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				XMFLOAT3 tmp_vel = Tool::NormalizeFloat3(m_player.vel);
				tmp_vel.x = -(tmp_vel.x * 0.4f) + static_cast<float>(rand() % 21 - 10) / 50;
				tmp_vel.y = static_cast<float>(rand() % 21 - 10) / 50;
				tmp_vel.z = static_cast<float>(rand() % 21 - 10) / 50;
				tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

				if (m_player.vel.x < 0)
				{
					m_booster->Add(5, m_offset.side[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.side[0]);
				}
				else if (0 < m_player.vel.x)
				{
					m_booster->Add(5, m_offset.side[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.side[1]);
				}
			}
		}
	}
}

void Player::BoosterGauge()
{
	// クールタイム出ないなら回復
	if (m_gauge.coolTime == 0)
	{
		m_player.boostGauge += m_gauge.add;
	}
	else
	{
		m_gauge.coolTime++;
		if (60 * 2 <= m_gauge.coolTime)
		{
			m_gauge.coolTime = 0;
		}
	}

	if (m_gauge.max < m_player.boostGauge)
	{
		m_player.boostGauge = m_gauge.max;
	}
}

bool Player::SearchTarget(const XMFLOAT3& target)
{
	// 範囲内なら
	if (Tool::LengthFloat3(m_player.pos, target) < 150.0f)
	{
		return true;
	}

	return false;
}
