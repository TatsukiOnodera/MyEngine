#pragma once
#include "BaseObject.h"

class Enemy : public BaseObject
{
private: //�������u����
//�R�}���h���X�g
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCmdList();
	//����n
	Input* input = Input::GetInstance();
	//�I�[�f�B�I
	Audio* audio = Audio::GetInstance();
	//�J����
	Camera* camera = Camera::GetInstance();
	//�f�o�b�O�e�L�X�g
	DebugText debugText;

private: //�����o�ϐ�


public: //�����o�֐�
	//�f�X�g���N�^
	~Enemy() override;

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;
};
