#pragma once
#include <DirectXMath.h>

class Camera
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Camera* GetInstance();

private: //�����o�ϐ�
	//�����_���W
	XMFLOAT3 target = { 0, 0, 0 };
	//�n�_���W
	XMFLOAT3 eye = { 0, 0, -50 };
	//������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	//�����_����n�_�܂ł̋���
	XMFLOAT3 distance = { 0, 0, 0 };
	//�r���[�s��
	XMMATRIX matView;
	//�ˉe�s��
	XMMATRIX matProjection;
	//�r���{�[�h�s��
	XMMATRIX matBillboard;
	//Y���r���{�[�h�s��
	XMMATRIX matBillboardY;
	//X���̊p�x
	float angleX = 0.0f;
	//Y���̊p�x
	float angleY = 0.0f;
	//�_�[�e�B�[�t���O
	bool dirty = false;
	//	�X�V������
	bool isDirty = false;

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="window_width">�E�B���h�E����</param>
	/// <param name="window_height">�E�B���h�E�c��</param>
	void Initialize(int window_width, int window_height);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �J�����̈ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveCamera(XMFLOAT3 move);

	/// <summary>
	/// �Ǐ]�J����
	/// </summary>
	/// <param name="target">�����_</param>
	/// <param name="eye">�����_����n�_�ւ̐���</param>
	/// <param name="addAngleX">X���̊p�x</param>
	/// <param name="addAngleY">Y���̊p�x</param>
void FollowUpCamera(XMFLOAT3 target, XMFLOAT3 eye, float addAngleX, float addAngleY);

	/// <summary>
	/// �J���������ɍ��W���ړ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ���</param>
	/// <returns>�ړ�������̍��W</returns>
	XMFLOAT3 ConvertWindowPos(XMFLOAT3 pos, XMFLOAT3 vec);

public: //�A�N�Z�b�T
	/// <summary>
	/// �J�������W�擾
	/// </summary>
	/// <returns>�J�������W</returns>
	XMFLOAT3 GetEye() { return eye; }

	/// <summary>
	/// �J�������W�Z�b�g
	/// </summary>
	/// <param name="eye">�J�������W</param>
	void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// �œ_���W�擾
	/// </summary>
	/// <returns>���_���W</returns>
	XMFLOAT3 GetTarget() { return target; }

	/// <summary>
	/// �œ_���W�Z�b�g
	/// </summary>
	/// <param name="target">�œ_���W</param>
	void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// �㉺�擾
	/// </summary>
	/// <returns>��̌���</returns>
	XMFLOAT3 GetUp() { return up; }

	/// <summary>
	/// �㉺�̃Z�b�g
	/// </summary>
	/// <param name="up">��̌���</param>
	void SetUp(XMFLOAT3 up);

	/// <summary>
	/// �����_����n�_�܂ł̋����擾
	/// </summary>
	/// <returns>�����_����n�_�܂ł̋���</returns>
	XMFLOAT3 GetDistance() { return distance; }

	/// <summary>
	/// �����_����n�_�܂ł̋����Z�b�g
	/// </summary>
	void SetDistance();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	XMMATRIX GetMatView() { return matView; }

	/// <summary>
	/// �ˉe�s��
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	XMMATRIX GetMatProject() { return matProjection; }

	/// <summary>
	/// �r���{�[�h�s��
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	XMMATRIX GetMatBillboard() { return matBillboard; }

	/// <summary>
	/// �J�����X�V�̐���
	/// </summary>
	/// <returns>����</returns>
	bool GetDirty() { return isDirty; };
};