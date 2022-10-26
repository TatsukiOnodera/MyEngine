#include "DirectXCommon.h"

#include "Player.h"

Camera* Player::s_camera = Camera::GetInstance();
Input* Player::s_input = Input::GetInstance();

Player::Player()
{
	m_object.reset(FbxObject3d::CreateFBXObject("PlayerBone"));
	//booster.reset(ParticleManager::Create("Particle/effect1.png"));
	booster.reset(ParticleManager::Create("Particle/FireParticle.png"));
	Initialize();
}

Player::~Player()
{
	
}

void Player::Initialize()
{
	// null�`�F�b�N
	if (m_object == nullptr)
	{
		assert(0);
	}

	// �X�e�[�^�X
	m_status.isAlive = true;
	m_status.isDash = false;
	m_status.HP = 20;
	m_status.pos = { 0, 0, -100 };
	m_status.vel = { 0, 0, 0 };

	// �_�b�V���̉����x
	m_dashAcc = 0;
	// �_�b�V���̎���
	m_dashTime = 0;
	// 1�t���[��������̉��Z�iDT = DashTime�j
	m_addDT = 8.0f;
	// �d�͎���
	m_gravityTime = 0;

	// �W�I�̍��W
	m_targetPos = { 0, 0, 0 };
	// ���b�N����
	m_isLock = false;
	// ���ˊԊu
	m_shotInterval = 0;
	// ���e��
	m_bulletCapacity = 20;
	// �����[�h�^�C��
	m_reloadTimer = 0;

	// �J�����ʒu�̐��ʉ�
	m_cameraInitialize = false;

	// �I�u�W�F�N�g
	m_object->SetPosition(m_status.pos);
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->SetRotation({ -90, 180, 0 });
	m_object->Update();
}

void Player::Update()
{
	// �����Ă���Ȃ�
	if (m_status.isAlive == true)
	{
		// �d�͎���
		m_gravityTime++;

		// �����x
		XMFLOAT3 acc = {};

		// �c���ړ�
		MovePlayer(acc);

		// �㉺�̈ړ�
		JumpPlayer(acc);

		// ���x�ɉ��������Z
		AddAcceleration(acc);

		// �_�b�V��
		DashPlayer();

		// �J���������ɂ����ϊ�
		m_status.pos = s_camera->ConvertWindowYPos(m_status.pos, m_status.vel);

		// ���W�Z�b�g
		m_object->SetPosition(m_status.pos);

		// �V���b�g
		ShotBullet();
	}

	// �e�̍X�V
	for (auto& m : playerBullets)
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
	
	if (0 < playerBullets.size())
	{
		for (auto& m : playerBullets)
		{
			m->Draw();
		}
	}
	
	Object3d::PostDraw();

	// FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	if (m_status.isAlive == true)
	{
		// ���@
		m_object->Draw();
	}

	FbxObject3d::PostDraw();

	// �p�[�e�B�N���`��
	ParticleManager::PreDraw(cmdList);

	XMFLOAT3 pos = m_status.pos;
	pos.y += 3.0f * m_object->GetScale().z;
	pos.z += -2.0f * m_object->GetScale().y;
	booster->SetTargetPos(pos);
	booster->Draw();
	
	ParticleManager::PostDraw();
}

void Player::MovePlayer(XMFLOAT3& acc)
{
	if (s_input->LeftStickAngle().x != 0 || s_input->LeftStickAngle().y != 0)
	{
		// ����
		acc.x += s_input->LeftStickAngle().x * c_accMove;
		acc.z += s_input->LeftStickAngle().y * c_accMove;
	}
	else
	{
		// ����
		m_status.vel.x *= c_decMove;
		m_status.vel.z *= c_decMove;

		// ���ȉ��Ȃ�0��
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
	// �W�����v
	if (s_input->PullLeftTrigger())
	{
		acc.y += c_accJump;
		m_gravityTime = 0;
	}

	// �d��
	acc.y += (-0.98f *0.5f) * powf(static_cast<float>(m_gravityTime) / 60, 2);
}

void Player::AddAcceleration(const XMFLOAT3& acc)
{
	m_status.vel.x += acc.x;
	m_status.vel.y += acc.y;
	m_status.vel.z += acc.z;

	// ���x�𒲐�
	// X��
	if (c_maxVelXZ < fabs(m_status.vel.x))
	{
		float tmp_div = fabs(m_status.vel.x) / c_maxVelXZ;
		m_status.vel.z /= tmp_div;
		m_status.vel.x /= tmp_div;
	}
	// Z��
	if (c_maxVelXZ < fabs(m_status.vel.z))
	{
		float tmp_div = fabs(m_status.vel.z) / c_maxVelXZ;
		m_status.vel.x /= tmp_div;
		m_status.vel.z /= tmp_div;
	}
	// Y��
	if (c_maxVelY < m_status.vel.y)
	{
		m_status.vel.y = c_maxVelY;
	}
}

void Player::DashPlayer()
{
	// �_�b�V��
	if(s_input->SwitchRightTrigger() && m_status.isDash == false && m_status.vel.x != 0 && m_status.vel.z != 0)
	{
		m_status.isDash = true;
	}

	// ����
	if (m_status.isDash == true)
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
			m_status.isDash = false;
		}

		float tmp_t = 0;
		if (0 < fabs(m_addDT))
		{
			tmp_t = (m_dashTime / 60) * (2 - (m_dashTime) / 60);

			XMFLOAT3 pos = m_status.pos;
			pos.y += 3.0f * m_object->GetScale().z;
			pos.z += -2.0f * m_object->GetScale().y;

			// ���K��
			XMFLOAT3 vel = m_status.vel;
			float tmp_len = Length(vel);
			vel.x = vel.x / tmp_len * -0.3f;
			vel.y = 0;
			vel.z = vel.z / tmp_len * -0.3f;
			vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, vel);
			for (int p = 0; p < 3; p++)
			{
				pos.x *= 1.0f + static_cast<float>(rand() % 11) / 10;
				pos.z *= 1.0f + static_cast<float>(rand() % 11) / 10;
				booster->Add(5, targetPos, vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.4f, 1.0f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 1.5f, 0.0f, true);
			}
		}
		else
		{
			tmp_t = (m_dashTime / 60) * (m_dashTime / 60);
		}

		m_dashAcc = 4.0f * tmp_t;

		m_status.vel.x *= 1 + m_dashAcc;
		m_status.vel.z *= 1 + m_dashAcc;
	}
	else
	{
		XMFLOAT3 pos = m_status.pos;
		pos.y += 3.0f * m_object->GetScale().z;
		pos.z += -2.0f * m_object->GetScale().y;

		// ���K��
		XMFLOAT3 vel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_status.vel);
		float tmp_len = Length(vel);
		vel.x = vel.x / tmp_len * -0.04f;
		vel.y = -fabs(vel.y / tmp_len * -0.04f);
		vel.z = vel.z / tmp_len * -0.04f;
		booster->Add(10, targetPos, vel, { 0, 0, 0 }, { 0.6f, 0.4f, 0.4f, 0.00001f }, { 0.4f, 0.2f, 0.6f, 0.0f }, 0.3f, 0.2f, true);
	}
}

void Player::ShotBullet()
{
	m_shotInterval++;

	if (0 < m_bulletCapacity && m_reloadTimer == 0)
	{
		// ����
		if (s_input->PushButton(BUTTON::RB) && 8 < m_shotInterval)
		{
			m_shotInterval = 0;
			
			XMFLOAT3 vel = {};
			if (m_isLock == true)
			{
				// �W�I�܂ł̕������擾
				vel.x = m_targetPos.x - m_status.pos.x;
				vel.y = m_targetPos.y - m_status.pos.y;
				vel.z = m_targetPos.z - m_status.pos.z;

				// �����𐳋K������10�{����
				float len = sqrtf(powf(vel.x, 2) + powf(vel.y, 2) + powf(vel.z, 2));
				vel.x = vel.x / len * 6;
				vel.y = vel.y / len * 6;
				vel.z = vel.z / len * 6;
			}
			else
			{
				vel = { 0, 0, 6 };

				// �J�����̕����ɕϊ�
				vel = s_camera->ConvertWindowXYPos({ 0, 0, 0 }, vel);
			}

			// �g���Ă��Ȃ��e��T��
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
		// �����[�hs
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
		// �g���Ă��Ȃ��̂����邩
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
	// �x��ĒǏ]
	XMFLOAT3 tPos = m_status.pos;
	XMFLOAT3 tVel = s_camera->ConvertWindowYPos({ 0, 0, 0 }, m_status.vel);
	// ���x������
	tPos.x -= tVel.x;
	tPos.y -= tVel.y;
	tPos.z -= tVel.z;

	// �Ǐ]�J����
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
		// �J�������ʉ�
		if (s_camera->MoveFront(3))
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

const float Player::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
