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
	// null�`�F�b�N
	if (m_object == nullptr)
	{
		assert(0);
	}

	// ����
	m_alive = true;
	// ���W
	m_pos = { 0, 0, -100 };
	// ���x
	m_vel = { 0, 0, 0 };
	// �������x
	m_accSpeed = 0.1f;
	// �����x
	m_decSpeed = 0.8f;
	// �W�����v�����x
	m_accJump = 0.1f;
	// �d�͎���
	m_gravityTime = 0;
	// �_�b�V���t���O
	m_isDash = false;
	// �_�b�V��������
	m_dashTimes = 1.0f;
	// �_�b�V��������̖����Z�l
	m_accDashTimes = 0.6f;
	// �J�����̉�]�p�x
	m_addAngle = 1.0f;
	// �J�����ʒu�̐��ʉ�
	m_cameraInitialize = false;

	// �W�I�̍��W
	m_targetPos = { 0, 0, 0 };
	// ���b�N����
	m_isLock = false;
	// ���ˊԊu
	m_shotInterval = 0;

	// �I�u�W�F�N�g
	m_object->SetPosition(m_pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	// �����x
	XMFLOAT3 acc = {};

	// �����Ă���Ȃ�
	if (m_alive == true)
	{
		// �d�͎���
		m_gravityTime++;

		// �ړ�
		MovePlayer(acc);

		// �W�����v
		JumpPlayer(acc);

		// �d��
		acc.y -= (9.8f / 10) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// ���x�ɉ��������Z
		AddAcceleration(acc);

		// �_�b�V��
		DashPlayer();

		// �J���������ɂ����ϊ�
		m_pos = s_camera->ConvertWindowYPos(m_pos, m_vel);

		// ���W�Z�b�g
		m_object->SetPosition(m_pos);

		// �V���b�g
		ShotBullet();
	}

	// �e�̍X�V
	for (auto& m : playerBullets)
	{
		m->Update();
	}

	// �J�������[�N
	MoveCamera();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);
	// FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	if (m_alive == true)
	{
		// ���@
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
		// ����
		m_vel.x *= m_dashTimes;
		m_vel.z *= m_dashTimes;

		// ������ɉ��Z
		m_dashTimes += m_accDashTimes;

		// �����l��5���傫���Ȃ�
		if (5.0f < m_dashTimes)
		{
			m_dashTimes = 5.0f;
			m_accDashTimes = -0.2f;
		}
		// �����l��1����������Ȃ�
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
			// �W�I�̍��W���擾
			vel.x = m_targetPos.x - m_pos.x;
			vel.y = m_targetPos.y - m_pos.y;
			vel.z = m_targetPos.z - m_pos.z;

			// ������1�ɂ���10�{����
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

void Player::MoveCamera()
{
	// �x��ĒǏ]
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
			cameraAngle.y += s_input->RightStickAngle().x * m_addAngle;
			cameraAngle.x -= s_input->RightStickAngle().y * m_addAngle;
		}
	}
	else
	{
		// �J�������ʉ�
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
