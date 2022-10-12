#include "Camera.h"
#include "WinApp.h"

using namespace DirectX;

Camera* Camera::GetInstance()
{
	static Camera camera;

	return &camera;
}

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Initialize()
{
	//�r���{�[�h�s�񏉊���
	m_matBillboard = XMMatrixIdentity();
	m_matBillboardY = XMMatrixIdentity();

	// �r���[�s��̐���
	m_dirty = true;
	Update();

	// �������e�ɂ��ˉe�s��̐���
	UpdateMatProjection();
}

void Camera::Update()
{
	if (m_dirty == true)
	{
		//���_
		XMVECTOR eyePosition = XMLoadFloat3(&m_eye);
		//�����_
		XMVECTOR targetPosition = XMLoadFloat3(&m_target);
		//�����
		XMVECTOR upVector = XMLoadFloat3(&m_up);

		//�J����Z��
		XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
		//�x�N�g���𐳋K��
		cameraAxisZ = XMVector3Normalize(cameraAxisZ);

		//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
		assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
		assert(!XMVector3IsInfinite(cameraAxisZ));
		assert(!XMVector3Equal(upVector, XMVectorZero()));
		assert(!XMVector3IsInfinite(upVector));

		//�J������X��
		XMVECTOR cameraAxisX;
		//X���͏������Z���̊O�ςŋ��߂�
		cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
		//�x�N�g���𐳋K��
		cameraAxisX = XMVector3Normalize(cameraAxisX);

		//�J������Y��
		XMVECTOR cameraAxisY;
		//Y����Z����X���̊O�ςŋ��߂�
		cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
		//�x�N�g���𐳋K��
		cameraAxisY = XMVector3Normalize(cameraAxisY);

		//�J������]�s��
		XMMATRIX matCameraRot;
		//�J�������W�n�����[���h���W�n�̕ϊ��s��
		matCameraRot.r[0] = cameraAxisX;
		matCameraRot.r[1] = cameraAxisY;
		matCameraRot.r[2] = cameraAxisZ;
		matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

		//�]�u�ɂ��t�s����v�Z
		m_matView = XMMatrixTranspose(matCameraRot);

		//���_���W��-1�����������W
		XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
		//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��
		XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition);
		XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition);
		XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition);
		//��̃x�N�g���ɂ܂Ƃ߂�
		XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

		//�r���[�s��ɕ��s�ړ�������ݒ�
		m_matView.r[3] = translation;

		//�r���{�[�h�s��
		m_matBillboard.r[0] = cameraAxisX;
		m_matBillboard.r[1] = cameraAxisY;
		m_matBillboard.r[2] = cameraAxisZ;
		m_matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

		//�J����X���AY���AZ��
		XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

		//X���͋���
		ybillCameraAxisX = cameraAxisX;
		//Y���̓��[���h���W�n��Y��
		ybillCameraAxisY = XMVector3Normalize(upVector);
		//Z����X����Y���̊O�ςŋ��߂�
		ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

		//Y�����r���{�[�h�s��
		m_matBillboardY.r[0] = ybillCameraAxisX;
		m_matBillboardY.r[1] = ybillCameraAxisY;
		m_matBillboardY.r[2] = ybillCameraAxisZ;
		m_matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);

		m_dirty = false;
		m_isDirty = true;
	}
	else
	{
		m_isDirty = false;
	}
}

void Camera::UpdateMatProjection()
{
	// �ŋߒl���ŉ��l����������Ύ~�߂�
	assert(m_nearZ < m_farZ);

	// �������e�ɂ��ˉe�s��̍X�V
	m_matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		static_cast<float>(WinApp::window_width) / WinApp::window_height,
		m_nearZ, m_farZ);
}

void Camera::SetEye(const XMFLOAT3& eye)
{
	this->m_eye = eye;

	m_dirty = true;
}

void Camera::SetTarget(const XMFLOAT3& target)
{
	this->m_target = target;

	m_dirty = true;
}

void Camera::SetUp(const XMFLOAT3& up)
{
	this->m_up = up;

	m_dirty = true;
}

void Camera::SetDistance(const XMFLOAT3& distance)
{
	m_eye.x = m_target.x + distance.x;
	m_eye.y = m_target.y + distance.y;
	m_eye.z = m_target.z + distance.z;

	m_distance.x = distance.x;
	m_distance.y = distance.y;
	m_distance.z = distance.z;

	m_dirty = true;
}

void Camera::SetNearFarZ(const float& nearZ, const float& farZ)
{
	this->m_nearZ = nearZ;
	this->m_farZ = farZ;

	UpdateMatProjection();

	m_dirty = true;
}

void Camera::MoveCamera(const XMFLOAT3& move)
{
	this->m_eye.x += move.x;
	this->m_eye.y += move.y;
	this->m_eye.z += move.z;
	this->m_target.x += move.x;
	this->m_target.y += move.y;
	this->m_target.z += move.z;

	m_dirty = true;
}

void Camera::FollowUpCamera(const XMFLOAT3& target, const XMFLOAT3& eyeDistance, const float& angleX, const float& angleY)
{
	//�����_�Z�b�g
	SetTarget(target);

	//�I�t�Z�b�g�x�N�g��
	XMVECTOR v0 = { eyeDistance.x, eyeDistance.y, eyeDistance.z, 0 };

	//��]�s��
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(angleX));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleY));

	//�����_����n�_�ւ̃x�N�g�������߂�
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//�����_�ɑ����Ď��_�����߂�
	XMFLOAT3 eyePosition = { target.x + V.m128_f32[0], target.y + V.m128_f32[1], target.z + V.m128_f32[2] };

	//�n�_���Z�b�g
	SetEye(eyePosition);
}

XMFLOAT3 Camera::ConvertWindowYPos(const XMFLOAT3& pos, const XMFLOAT3& vec, const float& angleY)
{
	//�ړ��x�N�g��
	XMVECTOR v0 = { vec.x, vec.y, vec.z, 0 };

	//��]�s��
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleY));

	//���ʂ����ƂɈړ������x�N�g���̌������o��
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//���̍��W�Ɉړ������x�N�g���𑫂�
	XMFLOAT3 position = { pos.x + V.m128_f32[0], pos.y + V.m128_f32[1], pos.z + V.m128_f32[2] };

	return position;
}

XMFLOAT3 Camera::ConvertWindowXYPos(const XMFLOAT3& pos, const XMFLOAT3& vec, const float& angleX, const float& angleY)
{
	//�ړ��x�N�g��
	XMVECTOR v0 = { vec.x, vec.y, vec.z, 0 };

	//��]�s��
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleX));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleY));

	//���ʂ����ƂɈړ������x�N�g���̌������o��
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//���̍��W�Ɉړ������x�N�g���𑫂�
	XMFLOAT3 position = { pos.x + V.m128_f32[0], pos.y + V.m128_f32[1], pos.z + V.m128_f32[2] };

	return position;
}

XMFLOAT2 Camera::Convert3DPosTo2DPos(const XMFLOAT3& pos)
{
	// �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
	float w = (float)WinApp::window_width / 2.0f;
	float h = (float)WinApp::window_height / 2.0f;

	XMMATRIX viewport = {
		w, 0, 0, 0,
		0, -h, 0, 0,
		0, 0, 1, 0,
		w, h, 0, 1
	};

	XMVECTOR tmp;
	tmp.m128_f32[0] = pos.x;
	tmp.m128_f32[1] = pos.y;
	tmp.m128_f32[2] = pos.z;
	tmp.m128_f32[3] = 1;

	tmp = XMVector3Transform(tmp, m_matView);
	tmp = XMVector3Transform(tmp, m_matProjection);

	tmp.m128_f32[0] /= tmp.m128_f32[2];
	tmp.m128_f32[1] /= tmp.m128_f32[2];
	tmp.m128_f32[2] /= tmp.m128_f32[2];

	XMVECTOR screenPos;
	screenPos = XMVector3Transform(tmp, viewport);

	XMFLOAT2 position = { screenPos.m128_f32[0], screenPos.m128_f32[1] };

	return position;
}

void Camera::SetCameraPosition(const XMFLOAT3& target)
{
	m_target.x = target.x;
	m_target.y = target.y;
	m_target.z = target.z;

	m_eye.x = target.x + m_distance.x;
	m_eye.y = target.y + m_distance.y;
	m_eye.z = target.z + m_distance.z;

	m_dirty = true;
}
