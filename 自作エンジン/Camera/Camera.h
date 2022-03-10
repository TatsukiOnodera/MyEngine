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
	static Camera* GetInstance();

private: //�����o�ϐ�
	//�n�_���W
	XMFLOAT3 eye; 
	//���ӓ_���W
	XMFLOAT3 target; 
	//������x�N�g��
	XMFLOAT3 up; 
	//�r���[�s��
	XMMATRIX matView;
	//�ˉe�s��
	XMMATRIX matProjection;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="eye">�J�������W</param>
	/// <param name="target">�œ_���W</param>
	/// <param name="up">�㉺���</param>
	Camera(XMFLOAT3 eye = { 0, 0, -25 }, XMFLOAT3 target = { 0, 0, 0 }, XMFLOAT3 up = { 0, 1, 0 });

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// ���_���
	/// </summary>
	XMFLOAT3 GetEye() { return eye; }
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }

	/// <summary>
	/// �œ_���
	/// </summary>
	XMFLOAT3 GetTarget() { return target; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }

	/// <summary>
	/// �㉺���
	/// </summary>
	XMFLOAT3 GetUp() { return up; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	XMMATRIX GetMatView() { return matView; }

	/// <summary>
	/// �ˉe�s��
	/// </summary>
	XMMATRIX GetMatProject() { return matProjection; }

	/// <summary>
	/// �J�����̈ړ�
	/// </summary>
	void MoveCamera(XMFLOAT3 move);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(int window_width, int window_height);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
};