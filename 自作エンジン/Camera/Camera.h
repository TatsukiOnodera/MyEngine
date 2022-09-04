#pragma once
#include <DirectXMath.h>

#include <list>

using namespace std;

class Camera
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Camera* GetInstance();

private: // �����o�ϐ�
	// �����_���W
	XMFLOAT3 m_target = { 0, 0, 0 };
	// �n�_���W
	XMFLOAT3 m_eye = { 0, 0, -50 };
	// ������x�N�g��
	XMFLOAT3 m_up = { 0, 1, 0 };
	// �����_����n�_�܂ł̋���
	XMFLOAT3 m_distance = { 0, 0, 0 };
	// �J�����̍ŋ�Z�l
	float m_nearZ = 0.1f;
	// �J�����̍ŉ�Z�l
	float m_farZ = 1000.0f;
	// �r���[�s��
	XMMATRIX m_matView;
	// �ˉe�s��
	XMMATRIX m_matProjection;
	// �r���{�[�h�s��
	XMMATRIX m_matBillboard;
	// Y���r���{�[�h�s��
	XMMATRIX m_matBillboardY;
	// X���̊p�x
	float m_angleX = 0.0f;
	// Y���̊p�x
	float m_angleY = 0.0f;
	// Z���̊p�x
	float m_angleZ = 0.0f;
	// �_�[�e�B�[�t���O
	bool m_dirty = false;
	//	 �X�V������
	bool m_isDirty = false;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �������e�ɂ��ˉe�s��̍X�V
	/// </summary>
	void UpdateMatProjection();

	/// <summary>
	/// �J�����̈ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveCamera(XMFLOAT3 move);

	/// <summary>
	/// �Ǐ]�J����
	/// </summary>
	/// <param name="target">�����_</param>
	/// <param name="eyeDistance">�����_����n�_�ւ̋���</param>
	/// <param name="addAngleX">X���̊p�x</param>
	/// <param name="addAngleY">Y���̊p�x</param>
void FollowUpCamera(XMFLOAT3 target, XMFLOAT3 eyeDistance, float addAngleX, float addAngleY);

	/// <summary>
	/// �J���������ɍ��W���ړ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ���</param>
	/// <returns>�ړ�������̍��W</returns>
	XMFLOAT3 ConvertWindowPos(XMFLOAT3 pos, XMFLOAT3 vec);

	/// <summary>
	/// 3D���W��2D���W�ɕϊ�
	/// </summary>
	/// <param name="pos">3D���W</param>
	/// <returns>2D���W</returns>
	XMFLOAT2 Convert3DPosTo2DPos(XMFLOAT3 pos);

	/// <summary>
	/// m_angle�̃��Z�b�g
	/// </summary>
	void InitializeAngle();

public: //�A�N�Z�b�T
	/// <summary>
	/// �J�������W�擾
	/// </summary>
	/// <returns>�J�������W</returns>
	XMFLOAT3 GetEye() { return m_eye; }

	/// <summary>
	/// �J�������W�Z�b�g
	/// </summary>
	/// <param name="eye">�J�������W</param>
	void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// �œ_���W�擾
	/// </summary>
	/// <returns>���_���W</returns>
	XMFLOAT3 GetTarget() { return m_target; }

	/// <summary>
	/// �œ_���W�Z�b�g
	/// </summary>
	/// <param name="target">�œ_���W</param>
	void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// �㉺�擾
	/// </summary>
	/// <returns>��̌���</returns>
	XMFLOAT3 GetUp() { return m_up; }

	/// <summary>
	/// �㉺�̃Z�b�g
	/// </summary>
	/// <param name="up">��̌���</param>
	void SetUp(XMFLOAT3 up);

	/// <summary>
	/// �����_����n�_�܂ł̋����擾
	/// </summary>
	/// <returns>�����_����n�_�܂ł̋���</returns>
	XMFLOAT3 GetDistance() { return m_distance; }

	/// <summary>
	/// �����_����n�_�܂ł̋����Z�b�g
	/// </summary>
	void SetDistance();

	/// <summary>
	/// �J�����̉�]�p���擾
	/// </summary>
	/// <returns>�O�����̃J�����̉�]�p</returns>
	XMFLOAT3 GetAngle() { return XMFLOAT3(m_angleX, m_angleY, m_angleZ); }

	/// <summary>
	/// �J�����̍ŋ߉�Z�l���擾
	/// </summary>
	/// <returns>�ŋ߉�Z�l</returns>
	XMFLOAT2 GetNearFarZ() { return XMFLOAT2(m_nearZ, m_farZ); }

	/// <summary>
	/// �J�����̍ŋ߉�Z�l���Z�b�g
	/// </summary>
	/// <param name="nearZ">�ŋߒl</param>
	/// <param name="farZ">�ŉ��l</param>
	void SetNearFarZ(float nearZ, float farZ);

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	XMMATRIX GetMatView() { return m_matView; }

	/// <summary>
	/// �ˉe�s��
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	XMMATRIX GetMatProject() { return m_matProjection; }

	/// <summary>
	/// �r���{�[�h�s��
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	XMMATRIX GetMatBillboard() { return m_matBillboard; }

	/// <summary>
	/// �J�����X�V�̐���
	/// </summary>
	/// <returns>����</returns>
	bool GetDirty() { return m_isDirty; };
};