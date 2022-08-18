#include "Player.h"

Input* Player::s_input = nullptr;
Camera* Player::s_camera = nullptr;

Player::Player(FbxObject3d* player)
{
	if (s_input == nullptr)
	{
		s_input = Input::GetInstance();
	}
	if (s_camera == nullptr)
	{
		s_camera = Camera::GetInstance();
	}
	m_object.reset(player);
	Initialize();
}

Player::~Player()
{

}

void Player::Initialize()
{
	m_alive = true;
	m_isDash = false;
	m_add0 = 0;

	assert(m_object);
	m_object->SetPosition({ 0, 0, -100 });
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

void Player::Update()
{
	if (m_alive)
	{
		XMFLOAT3 pVec = {};
		if (s_input->PushKey(DIK_D) || s_input->PushKey(DIK_A) || s_input->PushKey(DIK_W) || s_input->PushKey(DIK_S))
		{
			//�_�b�V��
			if (s_input->TriggerKey(DIK_SPACE))
			{
				m_isDash = true;
				m_add0 = 25;

				//�A�j���[�V����
				m_object->PlayAnimation(false);
			}

			//�x�N�g��
			pVec.x += (s_input->PushKey(DIK_D) - s_input->PushKey(DIK_A)) * 1;
			pVec.z += (s_input->PushKey(DIK_W) - s_input->PushKey(DIK_S)) * 1;
			//��������Ȃ�
			if (m_isDash)
			{
				pVec.x *= m_add0;
				pVec.z *= m_add0;

				m_add0 = m_add0 - 10;

				//�����x��0�ɂȂ�����
				if (m_add0 <= 0)
				{
					m_add0 = 0;
					m_isDash = false;
				}
			}
		} else
		{
			//�A�j���[�V����
			m_object->ResetAnimation();
		}
		//�J���������ɂ����ϊ�
		m_pos = m_object->GetPosition();
		m_pos = s_camera->ConvertWindowPos(m_pos, pVec);
		m_object->SetPosition(m_pos);
	}
}

void Player::Draw()
{
	if (m_alive)
	{
		m_object->Draw();
	}
}
