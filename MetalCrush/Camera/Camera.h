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
	// �J������X��]�p
	float m_angleX = 0.0;
	// �J������Y��]�p
	float m_angleY = 0.0;
	// �J������Z��]�p
	float m_angleZ = 0.0f;
	// �J�����̎���p
	float m_fovAngleY = 60.0f;
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
	void MoveCamera(const XMFLOAT3& move);

	/// <summary>
	/// �Ǐ]�J����
	/// </summary>
	/// <param name="target">�����_</param>
	/// <param name="eyeDistance">�����_����n�_�ւ̋���</param>
	/// <param name="addAngle">��]�����p�x</param>
	void FollowUpCamera(const XMFLOAT3& target, const XMFLOAT3& eyeDistance, const XMFLOAT3& addAngle = { 0, 0, 0 });

	/// <summary>
	/// �J������Y�������ɍ��W���ړ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ���</param>
	/// <param name="addAngleY">Y���̉�]�����p�x</param>
	/// <returns>�ړ�������̍��W</returns>
	XMFLOAT3 ConvertWindowYPos(const XMFLOAT3& pos, const XMFLOAT3& vec, const float addAngleY = 0.0f);

	/// <summary>
	/// �J������X����Y�������ɍ��W���ړ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ���</param>
	/// <param name="addAngle">��]�����p�x</param>
	/// <returns>�ړ�������̍��W</returns>
	XMFLOAT3 ConvertWindowXYPos(const XMFLOAT3& pos, const XMFLOAT3& vec, const XMFLOAT2& addAngle = { 0, 0 });

	/// <summary>
	/// 3D���W��2D���W�ɕϊ�
	/// </summary>
	/// <param name="pos">3D���W</param>
	/// <returns>2D���W</returns>
	XMFLOAT2 Convert3DPosTo2DPos(const XMFLOAT3& pos);

	/// <summary>
	/// ���E���ɂ��邩
	/// </summary>
	/// <param name="pos">����Ώۂ̍��W</param>
	/// <returns>����</returns>
	bool ObjectComeInSight(const XMFLOAT3& pos);

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
	void SetEye(const XMFLOAT3& eye)
	{
		this->m_eye = eye;

		m_dirty = true;
	}

	/// <summary>
	/// �œ_���W�擾
	/// </summary>
	/// <returns>���_���W</returns>
	XMFLOAT3 GetTarget() { return m_target; }

	/// <summary>
	/// �œ_���W�Z�b�g
	/// </summary>
	/// <param name="target">�œ_���W</param>
	void SetTarget(const XMFLOAT3& target)
	{
		this->m_target = target;

		m_dirty = true;
	}

	/// <summary>
	/// �㉺�擾
	/// </summary>
	/// <returns>��̌���</returns>
	XMFLOAT3 GetUp() { return m_up; }

	/// <summary>
	/// �㉺�̃Z�b�g
	/// </summary>
	/// <param name="up">��̌���</param>
	void SetUp(const XMFLOAT3& up)
	{
		this->m_up = up;

		m_dirty = true;
	}

	/// <summary>
	/// �����_����n�_�܂ł̋����擾
	/// </summary>
	/// <returns>�����_����n�_�܂ł̋���</returns>
	XMFLOAT3 GetDistance() { return m_distance; }

	/// <summary>
	/// �����_����n�_�܂ł̋����Z�b�g
	/// </summary>
	/// <param name="distance">����</param>
	void SetDistance(const XMFLOAT3& distance = { 0, 1, -5 })
	{
		m_distance.x = distance.x;
		m_distance.y = distance.y;
		m_distance.z = distance.z;

		m_eye.x = m_target.x + m_distance.x;
		m_eye.y = m_target.y + m_distance.y;
		m_eye.z = m_target.z + m_distance.z;

		m_dirty = true;
	}

	/// <summary>
	/// �J�����̉�]�p�̃Z�b�g
	/// </summary>
	/// <param name="cameraAngle">��]�p</param>
	void SetCameraAngle(const XMFLOAT3& cameraAngle = { 0, 0, 0 })
	{
		m_angleX = cameraAngle.x;
		m_angleY = cameraAngle.y;
		m_angleZ = cameraAngle.z;
	}

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
	void SetNearFarZ(const float nearZ, const float farZ)
	{
		this->m_nearZ = nearZ;
		this->m_farZ = farZ;

		UpdateMatProjection();

		m_dirty = true;
	}

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