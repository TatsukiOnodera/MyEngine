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
	// null�`�F�b�N
	if (m_object == nullptr)
	{
		assert(0);
	}

	// �����t���O
	m_alive = true;

	// �I�u�W�F�N�g���
	m_pos = { 0, 0, -50 };
	m_vel = { 0, 0, 0 };

	// �ړ�
	m_maxSpeed = 1.0f;

	// �_�b�V��
	m_isDash = false;
	m_dashSpeed = 2.0f;
	m_dashTime = 0;

	// �W�����v
	m_maxJumpSpeed = 1.0f;

	// �d��
	m_gravityTime = 0;

	// �I�u�W�F�N�g
	m_object->SetPosition(m_pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	// �����x
	XMFLOAT3 add = {};

	// �����Ă���Ȃ�
	if (m_alive == true)
	{
		// �ړ�
		{
			if (input->LeftStickAngle().x != 0 || input->LeftStickAngle().y != 0)
			{
				add.x = input->LeftStickAngle().x * 0.1f;
				add.z = input->LeftStickAngle().y * 0.1f;
			}
			else
			{
				m_vel.x *= 0.85f;
				m_vel.z *= 0.85f;
				if (fabs(m_vel.x) < 0.001f)
				{
					m_vel.x = 0;
				}
				if (fabs(m_vel.z) < 0.001f)
				{
					m_vel.z = 0;
				}
			}
		}

		// �d��
		{
			add.y = (-9.8f / 0.5f) * powf(static_cast<float>(m_gravityTime) / 60, 2);
			m_gravityTime++;
			if (60 < m_gravityTime)
			{
				m_gravityTime = 60;
			}
		}

		// �W�����v
		{
			if (input->PullLeftTrigger())
			{
				add.y += 0.05f;

				m_gravityTime = 0;
			}
		}

		// �x�N�g���ɉ��Z
		m_vel.x += add.x;
		m_vel.y += add.y;
		m_vel.z += add.z;
		if (m_maxSpeed < fabs(m_vel.x))
		{
			m_vel.x = (fabs(m_vel.x) / m_vel.x) * m_maxSpeed;
		}
		if (m_maxSpeed < fabs(m_vel.z))
		{
			m_vel.z = (fabs(m_vel.z) / m_vel.z) * m_maxSpeed;
		}
		if (m_maxJumpSpeed < m_vel.y)
		{
			m_vel.y = m_maxJumpSpeed;
		}

		// �_�b�V��
		{
			if (input->SwitchRightTrigger() && m_isDash == false)
			{
				m_isDash = true;
			}
			if (m_isDash == true)
			{
				m_vel.x += m_dashSpeed * (1 - m_dashTime) * (fabs(m_vel.x) / m_vel.x);
				m_vel.z += m_dashSpeed * (1 - m_dashTime) * (fabs(m_vel.z) / m_vel.z);

				m_dashTime += 0.05f;
				if (1 < m_dashTime)
				{
					m_dashTime = 0;
					m_isDash = false;
				}
			}
		}

		// �J���������ɂ����ϊ�
		m_pos = s_camera->ConvertWindowPos(m_object->GetPosition(), m_vel);
		
		// �ǂƂ̓����蔻��
		if (m_pos.x < -200 + 5)
		{
			m_pos.x = -200 + 5;
		}
		else if (200 - 5 < m_pos.x)
		{
			m_pos.x = 200 - 5;
		}
		if (m_pos.z < -200 + 5)
		{
			m_pos.z = -200 + 5;
		}
		else if (200 - 5 < m_pos.z)
		{
			m_pos.z = 200 - 5;
		}
		if (m_pos.y < -200 + 1.875f)
		{
			m_pos.y = -200 + 1.875f;

			m_vel.y = 0;
			m_gravityTime = 0;
		}

		// ���W�Z�b�g
		m_object->SetPosition(m_pos);
	}
	else
	{
		m_vel = {};
	}

	// �œ_����
	XMFLOAT3 tPos = m_pos;

	// �Ǐ]�J����
	XMFLOAT2 angle = {};
	if (input->RightStickAngle().x != 0 || input->RightStickAngle().y != 0)
	{
		angle.y += input->RightStickAngle().x * 0.8f;
		angle.x -= input->RightStickAngle().y * 0.8f;
	}
	s_camera->FollowUpCamera(tPos, s_camera->GetDistance(), angle.x, angle.y);
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

void Player::SetPosition(XMFLOAT3 position)
{
	m_pos = position;

	m_object->SetPosition(m_pos);
}
