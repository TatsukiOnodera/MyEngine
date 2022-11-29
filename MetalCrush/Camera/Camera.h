#pragma once
#include <DirectXMath.h>
#include <list>

class Camera
{
private: // �G�C���A�X
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
	// �J�����̊p�x
	XMFLOAT3 m_angle = { 0, 0, 0 };
	// �J�����̎���p
	float m_fovAngleY = 60.0f;
	// �J�����̍ŋ�Z�l
	float m_nearZ = 0.1f;
	// �J�����̍ŉ�Z�l
	float m_farZ = 20000.0f;
	// �r���[�s��
	XMMATRIX m_matView = {};
	// �ˉe�s��
	XMMATRIX m_matProjection = {};
	// �r���{�[�h�s��
	XMMATRIX m_matBillboard = {};
	// Y���r���{�[�h�s��
	XMMATRIX m_matBillboardY = {};
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
	void UpdateMatView();

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
	void FollowUpCamera(const XMFLOAT3& target, const XMFLOAT3& eyeDistance, const XMFLOAT3& addAngle);

	/// <summary>
	/// �J������Y�������ɍ��W���ړ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ���</param>
	/// <returns>�ړ�������̍��W</returns>
	XMFLOAT3 ConvertWindowYPos(const XMFLOAT3& pos, const XMFLOAT3& vec);

	/// <summary>
	/// �J������X����Y�������ɍ��W���ړ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ���</param>
	/// <returns>�ړ�������̍��W</returns>
	XMFLOAT3 ConvertWindowXYPos(const XMFLOAT3& pos, const XMFLOAT3& vec);

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
	inline XMFLOAT3& GetEye() { return m_eye; }

	/// <summary>
	/// �J�������W�Z�b�g
	/// </summary>
	/// <param name="eye">�J�������W</param>
	inline void SetEye(const XMFLOAT3& eye)
	{
		m_eye = eye;

		m_dirty = true;
	}

	/// <summary>
	/// �œ_���W�擾
	/// </summary>
	/// <returns>���_���W</returns>
	inline XMFLOAT3& GetTarget() { return m_target; }

	/// <summary>
	/// �œ_���W�Z�b�g
	/// </summary>
	/// <param name="target">�œ_���W</param>
	inline void SetTarget(const XMFLOAT3& target)
	{
		m_target = target;

		m_dirty = true;
	}

	/// <summary>
	/// �㉺�擾
	/// </summary>
	/// <returns>��̌���</returns>
	inline XMFLOAT3& GetUp() { return m_up; }

	/// <summary>
	/// �㉺�̃Z�b�g
	/// </summary>
	/// <param name="up">��̌���</param>
	inline void SetUp(const XMFLOAT3& up)
	{
		m_up = up;

		m_dirty = true;
	}

	/// <summary>
	/// �����_����n�_�܂ł̋����擾
	/// </summary>
	/// <returns>�����_����n�_�܂ł̋���</returns>
	inline XMFLOAT3& GetDistance() { return m_distance; }

	/// <summary>
	/// �����_����n�_�܂ł̋����Z�b�g
	/// </summary>
	/// <param name="distance">����</param>
	void SetDistance(const XMFLOAT3& distance = { 0, 1, -5 });

	/// <summary>
	/// �J�����̉�]�p�̃Z�b�g
	/// </summary>
	/// <param name="cameraAngle">��]�p</param>
	inline void SetCameraAngles(const XMFLOAT3& cameraAngle = { 0, 0, 0 })
	{
		m_angle.x = cameraAngle.x;
		m_angle.y = cameraAngle.y;
		m_angle.z = cameraAngle.z;
	}

	/// <summary>
	/// �J�����̍ŋ߉�Z�l���擾
	/// </summary>
	/// <returns>�ŋ߉�Z�l</returns>
	inline XMFLOAT2& GetNearFarZ() { return XMFLOAT2(m_nearZ, m_farZ); }

	/// <summary>
	/// �J�����̍ŋ߉�Z�l���Z�b�g
	/// </summary>
	/// <param name="nearZ">�ŋߒl</param>
	/// <param name="farZ">�ŉ��l</param>
	void SetNearFarZ(const float nearZ, const float farZ);

	/// <summary>
	/// �J�����̉�]�p�x���擾
	/// </summary>
	/// <returns>��]�p�x</returns>
	inline XMFLOAT3& GetCameraAngles() { return m_angle; }

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	inline XMMATRIX& GetMatView() { return m_matView; }

	/// <summary>
	/// �ˉe�s��
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	inline XMMATRIX& GetMatProject() { return m_matProjection; }

	/// <summary>
	/// �r���{�[�h�s��
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	inline XMMATRIX& GetMatBillboard() { return m_matBillboard; }

	/// <summary>
	/// �J�����X�V�̐���
	/// </summary>
	/// <returns>����</returns>
	inline bool GetDirty() { return m_isDirty; };
};