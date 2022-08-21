#include "Player.h"
#include "DirectXCommon.h"

Input* Player::s_input = Input::GetInstance();
Camera* Player::s_camera = Camera::GetInstance();

Player::Player()
{
	
}

Player::~Player()
{

}

Player* Player::Create()
{
	Player* player = new Player;

	player->Initialize();

	return player;
}

void Player::Initialize()
{
	m_object.reset(FbxObject3d::CreateFBXObject("player"));

	m_alive = true;
	m_isDash = false;
	m_add0 = 0;

	assert(m_object);
	m_object->SetPosition(m_pos);
	m_object->SetRotation({ -90, 0, 0 });
	m_object->SetScale({ 0.25f, 0.25f, 0.25f });
	m_object->Update();
}

bool Player::Update(XMFLOAT3 enemyPos)
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
				m_add0 = 7.5;

				//�A�j���[�V����
				m_object->PlayAnimation(false);
			}

			//�x�N�g��
			pVec.x += (s_input->PushKey(DIK_D) - s_input->PushKey(DIK_A)) * 0.75;
			pVec.z += (s_input->PushKey(DIK_W) - s_input->PushKey(DIK_S)) * 0.75;
			//��������Ȃ�
			if (m_isDash)
			{
				pVec.x *= m_add0;
				pVec.z *= m_add0;

				m_add0 = m_add0 - 0.75;

				//�����x��0�ɂȂ�����
				if (m_add0 <= 0)
				{
					m_add0 = 0;
					m_isDash = false;
				}
			}
		}
		else
		{
			//�A�j���[�V����
			m_object->ResetAnimation();
		}

		//�J���������ɂ����ϊ�
		m_pos = m_object->GetPosition();
		m_pos = s_camera->ConvertWindowPos(m_pos, pVec);
		m_object->SetPosition(m_pos);

		//�e�̔���
		if (s_input->TriggerKey(DIK_RETURN) && Length(enemyPos, m_pos) < 90 && effectTimer == 0)
		{
			float bulletSpeed = 6;
			//���ׂĎg���Ă��邩
			if (UsingAllBullet())
			{
				XMFLOAT3 len = { enemyPos.x - m_pos.x, enemyPos.y - m_pos.y, enemyPos.z - m_pos.z };
				len.x /= Length(enemyPos, m_pos);
				len.y /= Length(enemyPos, m_pos);
				len.z /= Length(enemyPos, m_pos);
				len.x *= bulletSpeed;
				len.y *= bulletSpeed;
				len.z *= bulletSpeed;
				bullet.emplace_back(Bullet::Create(m_pos, len, true));
			}
			else
			{
				for (auto& m : bullet)
				{
					if (m->GetAlive() == false)
					{
						XMFLOAT3 len = { enemyPos.x - m_pos.x, enemyPos.y - m_pos.y, enemyPos.z - m_pos.z };
						len.x /= Length(enemyPos, m_pos);
						len.y /= Length(enemyPos, m_pos);
						len.z /= Length(enemyPos, m_pos);
						len.x *= bulletSpeed;
						len.y *= bulletSpeed;
						len.z *= bulletSpeed;
						m->Initialize(m_pos, len, true);
						break;
					}
				}
			}
		}
		else if (effectTimer > 0)
		{
			effectTimer++;
		}

		//�p������
		XMFLOAT3 rot = {};
		rot.x = -90;
		rot.y += Camera::GetInstance()->GetAngle().y;
		m_object->SetRotation(rot);

		//�e�̍X�V
		bool f = false;
		for (auto& m : bullet)
		{
			if (m->Update(enemyPos))
			{
				f = true;
			}
		}

		return f;
	}
	else
	{
		return false;
	}
}

void Player::Draw()
{
	if (m_alive && effectTimer % 5 == 0)
	{
		if (effectTimer > 60)
		{
			effectTimer = 0;
		}
		m_object->Draw();
	}

	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCmdList());

	for (const auto& m : bullet)
	{
		m->Draw();
	}

	Object3d::PostDraw();
}

bool Player::UsingAllBullet()
{
	for (const auto& m : bullet)
	{
		if (m->GetAlive() == false)
		{
			return false;
		}
	}

	return true;
}

const float Player::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

void Player::SetEffectTimer()
{
	effectTimer = 1;
}
