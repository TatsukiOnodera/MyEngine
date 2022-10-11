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
	m_pos = { 0, 0, -50 };
	// ���x
	m_vel = { 0, 0, 0 };
	// �������x
	m_accSpeed = 0.1f;
	// �����x
	m_decSpeed = 0.8f;
	// �W�����v�����x
	m_accJumpSpeed = 0.1f;
	// �d�͎���
	m_gravityTime = 0;
	// �_�b�V���t���O
	m_isDash = false;
	// �_�b�V��������
	m_dashTimes = 1.0f;
	// �_�b�V��������̖����Z�l
	m_accDashTimes = 0.6f;
	// �J�����̊p�x
	m_angle = { 0, 0, 0 };
	// �J�����̉�]�p�x
	m_addAngle = 1.0f;
	// �J�����ʒu�̐��ʉ�
	m_cameraInitialize = false;

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

		// �W�����v
		if (s_input->PullLeftTrigger())
		{
			acc.y += m_accJumpSpeed;
			m_gravityTime = 0;
		}
		acc.y -= (9.8f / 5) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// ���x�ɉ��������Z
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

		// �_�b�V��
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
			if (6.0f < m_dashTimes)
			{
				m_dashTimes = 6.0f;
				m_accDashTimes = -0.3f;
			}
			// �����l��1����������Ȃ�
			else if (m_dashTimes < 1.0f)
			{
				m_dashTimes = 1.0f;
				m_accDashTimes = 0.6f;
				m_isDash = false;
			}
		}

		// �J���������ɂ����ϊ�
		m_pos = s_camera->ConvertWindowPos(m_pos, m_vel, m_angle.y);

		// ���W�Z�b�g
		m_object->SetPosition(m_pos);
	}

	// �J�������[�N
	XMFLOAT3 tPos = m_pos;
	XMFLOAT3 tVel = s_camera->ConvertWindowPos({ 0, 0, 0 }, m_vel, m_angle.y);
	tPos.x -= tVel.x;
	tPos.y -= tVel.y;
	tPos.z -= tVel.z;

	// �Ǐ]�J����
	if (m_cameraInitialize == false)
	{
		if (s_input->PushButton(BUTTON::R_STICK))
		{
			m_cameraInitialize = true;
		}
		else if (s_input->RightStickAngle().x != 0 || s_input->RightStickAngle().y != 0)
		{
			// Y��
			m_angle.y += s_input->RightStickAngle().x * m_addAngle;
			if (m_angle.y < 0)
			{
				m_angle.y += 360;
			}
			else if (360 < m_angle.y)
			{
				m_angle.y -= 360;
			}
			// X��
			m_angle.x += s_input->RightStickAngle().y * m_addAngle;
			if (m_angle.x < 0)
			{
				m_angle.x += 360;
			}
			else if (360 < m_angle.x)
			{
				m_angle.x -= 360;
			}
		}
	}
	else
	{
		// �J�������ʉ�
		if (CameraInitialize(m_angle.x, m_angle.y) == true)
		{
			m_cameraInitialize = false;
		}
	}
	s_camera->FollowUpCamera(tPos, s_camera->GetDistance(), m_angle.x, m_angle.y);
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_alive == true)
	{
		// OBJ�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);



		Object3d::PostDraw();

		// FBX�I�u�W�F�N�g
		FbxObject3d::PreDraw(cmdList);

		// ���@
		m_object->Draw();

		FbxObject3d::PostDraw();
	}
}

void Player::OnCollision()
{
	m_vel.y = 0;
	m_gravityTime = 0;
}

bool Player::CameraInitialize(float& angleX, float& angleY)
{
	// X��
	if (angleX < 180)
	{
		angleX -= 1.0f;
		if (angleX <= 0)
		{
			angleX = 0;
		}
	}
	else
	{
		angleX += 1.0f;
		if (360 <= angleX)
		{
			angleX = 0;
		}
	}

	// Y��
	if (angleY < 180)
	{
		angleY -= 1.0f;
		if (angleY <= 0)
		{
			angleY = 0;
		}
	}
	else
	{
		angleY += 1.0f;
		if (360 <= angleY)
		{
			angleY = 0;
		}
	}

	// ���0�Ȃ�
	if (angleX == 0 && angleY == 0)
	{
		return true;
	}

	return false;
}

void Player::SetPosition(const XMFLOAT3& position)
{
	m_pos = position;

	m_object->SetPosition(m_pos);
}
