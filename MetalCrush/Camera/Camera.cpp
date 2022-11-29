#include "Camera.h"
#include "WinApp.h"

using namespace std;
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
	UpdateMatView();

	// �������e�ɂ��ˉe�s��̐���
	UpdateMatProjection();
}

void Camera::UpdateMatView()
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
		XMConvertToRadians(m_fovAngleY),
		static_cast<float>(WinApp::window_width) / WinApp::window_height,
		m_nearZ, m_farZ);
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

void Camera::FollowUpCamera(const XMFLOAT3& target, const XMFLOAT3& eyeDistance, const XMFLOAT3& addAngle)
{
	//�����_�Z�b�g
	SetTarget(target);

	//�I�t�Z�b�g�x�N�g��
	XMVECTOR v0 = { eyeDistance.x, eyeDistance.y, eyeDistance.z, 0 };

	//��]�s��
	m_angle.x += addAngle.x;
	if (360 < m_angle.x)
	{
		m_angle.x -= 360;
	}
	else if (m_angle.x < 0)
	{
		m_angle.x += 360;
	}

	m_angle.y += addAngle.y;
	if (360 < m_angle.y)
	{
		m_angle.y -= 360;
	}
	else if (m_angle.y < 0)
	{
		m_angle.y += 360;
	}

	m_angle.z += addAngle.z;
	if (360 < m_angle.z)
	{
		m_angle.z -= 360;
	}
	else if (m_angle.z < 0)
	{
		m_angle.z += 360;
	}

	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(m_angle.x));
	rotM *= XMMatrixRotationY(XMConvertToRadians(m_angle.y));
	rotM *= XMMatrixRotationZ(XMConvertToRadians(m_angle.z));

	//�����_����n�_�ւ̃x�N�g�������߂�
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//�����_�ɑ����Ď��_�����߂�
	XMFLOAT3 eyePosition = { target.x + V.m128_f32[0], target.y + V.m128_f32[1], target.z + V.m128_f32[2] };

	//�n�_���Z�b�g
	SetEye(eyePosition);
}

XMFLOAT3 Camera::ConvertWindowYPos(const XMFLOAT3& pos, const XMFLOAT3& vec)
{
	//�ړ��x�N�g��
	XMVECTOR v0 = { vec.x, vec.y, vec.z, 0 };

	// ��]�s��
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(m_angle.y));

	//���ʂ����ƂɈړ������x�N�g���̌������o��
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//���̍��W�Ɉړ������x�N�g���𑫂�
	XMFLOAT3 position = { pos.x + V.m128_f32[0], pos.y + V.m128_f32[1], pos.z + V.m128_f32[2] };

	return position;
}

XMFLOAT3 Camera::ConvertWindowXYPos(const XMFLOAT3& pos, const XMFLOAT3& vec)
{
	//�ړ��x�N�g��
	XMVECTOR v0 = { vec.x, vec.y, vec.z, 0 };

	//��]�s��
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(m_angle.x));
	rotM *= XMMatrixRotationY(XMConvertToRadians(m_angle.y));

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

	// ���W�ւ̃x�N�g��
	XMVECTOR tmp = { pos.x, pos.y, pos.z, 1 };

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

bool Camera::ObjectComeInSight(const XMFLOAT3& pos)
{
	XMVECTOR tmp = { pos.x, pos.y, pos.z, 1 };

	tmp = XMVector3Transform(tmp, m_matView);
	tmp = XMVector3Transform(tmp, m_matProjection);

	tmp.m128_f32[0] /= tmp.m128_f32[3];
	tmp.m128_f32[1] /= tmp.m128_f32[3];
	tmp.m128_f32[2] /= tmp.m128_f32[3];

	if (1.0f < fabs(tmp.m128_f32[0]) || 1.0f < fabs(tmp.m128_f32[1]) || 1.0f < fabs(tmp.m128_f32[2]))
	{
		return false;
	}

	return true;
}

void Camera::SetDistance(const XMFLOAT3& distance)
{
	m_distance.x = distance.x;
	m_distance.y = distance.y;
	m_distance.z = distance.z;

	m_eye.x = m_target.x + m_distance.x;
	m_eye.y = m_target.y + m_distance.y;
	m_eye.z = m_target.z + m_distance.z;

	m_dirty = true;
}

void Camera::SetNearFarZ(const float nearZ, const float farZ)
{
	m_nearZ = nearZ;
	m_farZ = farZ;

	UpdateMatProjection();

	m_dirty = true;
}