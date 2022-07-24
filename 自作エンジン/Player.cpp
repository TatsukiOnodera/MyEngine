#include "Player.h"

Player::~Player()
{

}

void Player::Initialize()
{
	m_player.reset(FbxObject3d::CreateFBXObject("Human"));

	m_isDash = false;
	m_add0 = 0;

	float playerSize = 0.5f;
	m_player->SetPosition({ 0, 0, -100 });
	m_player->SetRotation({ -90, 0, 0 });
	m_player->SetScale({ playerSize, playerSize, playerSize });
	m_player->Update();
}

void Player::Update()
{
	//�ړ�
	XMFLOAT3 vec = { 0, 0, 0 };
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		//�_�b�V��
		if (input->TriggerKey(DIK_SPACE))
		{
			isDash = true;
			add0 = 25;

			//�A�j���[�V����
			fbxObject->PlayAnimation(false);
		}

		//�x�N�g��
		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
		//��������Ȃ�
		if (isDash)
		{
			vec.x *= add0;
			vec.z *= add0;

			add0 = add0 - 10;

			//�����x��0�ɂȂ�����
			if (add0 <= 0)
			{
				add0 = 0;
				isDash = false;
			}
		}
	} else
	{
		//�A�j���[�V����
		fbxObject->ResetAnimation();
	}
	//�J���������ɂ����ϊ�
	XMFLOAT3 pos = fbxObject->GetPosition();
	pos = camera->ConvertWindowPos(pos, vec);
	fbxObject->SetPosition(pos);
}

void Player::Draw()
{


	//FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	m_player->Draw();

	FbxObject3d::PostDraw();
}
