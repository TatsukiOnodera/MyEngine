#include "DirectXCommon.h"
#include "Model.h"
#include "Player.h"
#include "SphereCollider.h"

#include <Tool.h>
#include <time.h>

Camera* Player::s_camera = Camera::GetInstance();
Input* Player::s_input = Input::GetInstance();

using namespace DirectX;

Player::Player(Model* playerModel, Model* bulletModel)
{
	// ���f���̓ǂݍ���
	m_bulletModel = bulletModel;
	m_playerOBJ.reset(Object3d::Create(Model::Create("PlayerEX")));
	m_booster.reset(ParticleManager::Create("Particle/FireParticle.png"));

	// ������
	Initialize();
}

Player::~Player()
{

}

void Player::Initialize()
{
	// null�`�F�b�N
	if (m_playerOBJ == nullptr || m_booster == nullptr)
	{
		assert(0);
	}

	// �v���C���[
	m_player.isAlive = true;
	m_player.isDash = false;
	m_player.HP = 20;
	m_player.pos = { 0, 0, -100 };
	m_player.vel = { 0, 0, 0 };

	// �ʏ�ړ�
	m_move.acc = 0.0325f;
	m_move.dec = 0.9f;
	m_move.maxVelXZ = 0.8f;

	// �W������
	m_jump.acc = 0.025f;
	m_jump.maxVelY = 0.8f;

	// �_�b�V��
	m_dash.acc = 0;
	m_dash.time = 0;
	m_dash.DT = 8.0f;

	// �d�͎���
	m_gravityTime = 0;

	// �o���b�g
	m_bullet.targetPos = { 0, 0, 0 };
	m_bullet.num = c_maxBulletNum;
	m_bullet.speed = 7;
	m_bullet.interval = 0;
	m_bullet.reloadTimer = 0;
	m_bullet.isReload = false;
	m_bullet.isLock = false;

	// �u�[�X�^�[�I�t�Z�b�g
	for (int i = 0; i < 2; i++)
	{
		m_offset.main[i] =XMFLOAT3(0, 0, 0);
		m_offset.main[i] = XMFLOAT3(0, 0, 0);
		m_offset.back[i] = XMFLOAT3(0, 0, 0);
	}
	XMFLOAT3 mOffset = { 0.25f, 0.75f, -0.5f };
	m_offset.mainOffset[0] = { -mOffset.x * m_playerOBJ->GetScale().x, mOffset.y * m_playerOBJ->GetScale().y, mOffset.z * m_playerOBJ->GetScale().z};
	m_offset.mainOffset[1] = { mOffset.x * m_playerOBJ->GetScale().x, mOffset.y * m_playerOBJ->GetScale().y, mOffset.z * m_playerOBJ->GetScale().z };
	XMFLOAT3 sOffset = { 0.75f, 1.0f, 0 };
	m_offset.sideOffset[0] = { -sOffset.x * m_playerOBJ->GetScale().x, sOffset.y * m_playerOBJ->GetScale().y, sOffset.z * m_playerOBJ->GetScale().z };
	m_offset.sideOffset[1] = { sOffset.x * m_playerOBJ->GetScale().x, sOffset.y * m_playerOBJ->GetScale().y, sOffset.z * m_playerOBJ->GetScale().z };
	XMFLOAT3 bOffset = { 0.25f, 0, 0.25f };
	m_offset.backOffset[0] = { -bOffset.x * m_playerOBJ->GetScale().x, bOffset.y * m_playerOBJ->GetScale().z, bOffset.z * m_playerOBJ->GetScale().y };
	m_offset.backOffset[1] = { bOffset.x * m_playerOBJ->GetScale().x, bOffset.y * m_playerOBJ->GetScale().z, bOffset.z * m_playerOBJ->GetScale().y };

	// �J����
	m_camera.addAngle = 1.0f;

	// �I�u�W�F�N�g
	m_playerOBJ->SetPosition(m_player.pos);
	m_playerOBJ->SetScale({ 0.25f, 0.25f, 0.25f });
	m_playerOBJ->SetCollider(new SphereCollider({ 0, 0, 0 }, 3));
	m_playerOBJ->Update();
}

void Player::Update()
{
	// �����x
	XMFLOAT3 acc = {};	

	// �����Ă���Ȃ�
	if (m_player.isAlive == true)
	{
		// �c���ړ�
		MovePlayer(acc);

		// �㉺�̈ړ�
		JumpPlayer(acc);

		// �d��
		acc.y += (-0.98f * 0.5f) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// �d�͎���
		m_gravityTime++;

		// ���x�ɉ��������Z
		NormalizeVel(acc);

		// �_�b�V��
		DashPlayer();

		// �J���������ɂ����ϊ�
		m_player.pos = s_camera->ConvertWindowYPos(m_player.pos, m_player.vel);

		// ���W�Z�b�g
		m_playerOBJ->SetPosition(m_player.pos);

		// �V���b�g
		ShotBullet();

		// �I�t�Z�b�g�̍X�V
		UpdateOffset();

		if (m_player.isDash == false)
		{
			// ���C���u�[�X�^�[�̃p�[�e�B�N��
			MainBooster();

			// �T�C�h�u�[�X�^�[�̃p�[�e�B�N��
			SideBooster();

			// �o�b�N�u�[�X�^�[�̃p�[�e�B�N��
			BacktBooster();
		}
		else
		{
			// �_�b�V�����̃p�[�e�B�N��
			DashBooster(acc);
		}
	}

	// �e�̍X�V
	for (auto& m : m_playerBullets)
	{
		m->Update();
	}

	// �J�������[�N
	CameraWork();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);
	
	for (auto& m : m_playerBullets)
	{
		m->Draw();
	}
	if (m_player.isAlive == true)
	{
		m_playerOBJ->Draw();
	}
	
	Object3d::PostDraw();

	// �p�[�e�B�N���`��
	m_booster->Draw(cmdList);
}

void Player::MovePlayer(XMFLOAT3& acc)
{
	if (s_input->LeftStickAngle().x != 0 || s_input->LeftStickAngle().y != 0)
	{
		// ����
		acc.x += s_input->LeftStickAngle().x * m_move.acc;
		acc.z += s_input->LeftStickAngle().y * m_move.acc;
	}
	else
	{
		// ����
		m_player.vel.x *= m_move.dec;
		m_player.vel.z *= m_move.dec;

		// ���ȉ��Ȃ�0��
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
	// �W�����v
	if (s_input->PullLeftTrigger())
	{
		acc.y += m_jump.acc;
		m_gravityTime = 0;
	}
}

void Player::DashPlayer()
{
	// �_�b�V��
	if(s_input->SwitchRightTrigger() && m_player.isDash == false && m_player.vel.x != 0 && m_player.vel.z != 0)
	{
		m_player.isDash = true;
	}

	// ����
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

		m_dash.acc = 4.0f * tmp_t;

		m_player.vel.x *= 1 + m_dash.acc;
		m_player.vel.z *= 1 + m_dash.acc;
	}
}

void Player::NormalizeVel(XMFLOAT3& acc)
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
	// ���ˊԊu
	m_bullet.interval++;

	// ����
	if (s_input->PushButton(BUTTON::RB) && 8 < m_bullet.interval && m_bullet.isReload == false)
	{
		m_bullet.interval = 0;

		XMFLOAT3 vel = {};
		if (m_bullet.isLock == true)
		{
			// �W�I�܂ł̕������擾
			vel.x = m_bullet.targetPos.x - m_player.pos.x;
			vel.y = m_bullet.targetPos.y - m_player.pos.y;
			vel.z = m_bullet.targetPos.z - m_player.pos.z;

			// �����𐳋K������
			vel = Tool::Normalize3(vel);
			vel.x *= m_bullet.speed;
			vel.y *= m_bullet.speed;
			vel.z *= m_bullet.speed;
		}
		else
		{
			// �܂�������΂�
			vel = { 0, 0, m_bullet.speed };

			// �J�����̕����ɕϊ�
			vel = s_camera->ConvertWindowXYPos({ 0, 0, 0 }, vel);
		}

		m_playerBullets.emplace_back(new Bullet(m_player.pos, vel, true, m_bulletModel));
		m_bullet.num--;

		if (m_bullet.num == 0)
		{
			m_bullet.num = c_maxBulletNum;
			m_bullet.isReload = true;
		}
	}
	// �����[�h
	else if (m_bullet.isReload == true)
	{
		m_bullet.reloadTimer++;
		if (60 < m_bullet.reloadTimer)
		{
			m_bullet.reloadTimer = 0;
			m_bullet.isReload = false;
		}
	}
}

void Player::CameraWork()
{
	// �x��ĒǏ]
	XMFLOAT3 tPos = m_player.pos;
	XMFLOAT3 tVel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_player.vel);
	// ���x������
	tPos.x -= tVel.x;
	tPos.y -= tVel.y;
	tPos.z -= tVel.z;

	// �Ǐ]�J����
	XMFLOAT3 cameraAngle = {};
	if (s_input->RightStickAngle().x != 0 || s_input->RightStickAngle().y != 0)
	{
		cameraAngle.y += s_input->RightStickAngle().x * m_camera.addAngle;
		cameraAngle.x -= s_input->RightStickAngle().y * m_camera.addAngle;
	}
	s_camera->FollowUpCamera(tPos, s_camera->GetDistance(), cameraAngle);

	// �p������
	XMFLOAT3 tmp_rot = m_playerOBJ->GetRotation();
	tmp_rot.y += cameraAngle.y;
	m_playerOBJ->SetRotation(tmp_rot);
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
	if (m_jump.maxVelY / 3 < m_player.vel.y || m_move.maxVelXZ / 3 < m_player.vel.z)
	{
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 tmp_vel = Tool::Normalize3(m_player.vel);
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
	if (m_move.maxVelXZ / 3 < fabs(m_player.vel.x))
	{
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 tmp_vel = Tool::Normalize3(m_player.vel);
			tmp_vel.x = -(tmp_vel.x * 0.1f) + static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel.y = static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel.z = static_cast<float>(rand() % 3 - 1) / 100;
			tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

			if (tmp_vel.x < 0)
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
	if (m_player.vel.z < -m_move.maxVelXZ / 3)
	{
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 tmp_vel = Tool::Normalize3(m_player.vel);
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
					XMFLOAT3 tmp_vel = Tool::Normalize3(m_player.vel);
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
					XMFLOAT3 tmp_vel = Tool::Normalize3(m_player.vel);
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
				XMFLOAT3 tmp_vel = Tool::Normalize3(m_player.vel);
				tmp_vel.x = -(tmp_vel.x * 0.4f) + static_cast<float>(rand() % 21 - 10) / 50;
				tmp_vel.y = static_cast<float>(rand() % 21 - 10) / 50;
				tmp_vel.z = static_cast<float>(rand() % 21 - 10) / 50;
				tmp_vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, tmp_vel);

				if (tmp_vel.x < 0)
				{
					m_booster->Add(5, m_offset.side[0], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.side[0]);
				}
				else
				{
					m_booster->Add(5, m_offset.side[1], tmp_vel, { 0, 0, 0 }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 0.0f, 0.2f, 0.4f, 0.0f }, 0.3f, 0.9f, true, &m_offset.side[1]);
				}
			}
		}
	}
}
