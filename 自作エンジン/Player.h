#pragma once
#include "BaseObject.h"
#include "FbxObject3d.h"

class Player : public BaseObject
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
	//�I�u�W�F�N�g
	std::unique_ptr<FbxObject3d> m_player = nullptr;

	//��������
	bool m_isDash;
	//���������l
	float m_add0;

public: //�����o�֐�
	//�f�X�g���N�^
	~Player() override;

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;
};
