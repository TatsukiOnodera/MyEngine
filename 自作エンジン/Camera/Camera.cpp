#include "Camera.h"

using namespace DirectX;

Camera* Camera::GetInstance()
{
	static Camera camera;
	return &camera;
}

Camera::Camera(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;

	matBillboard = XMMatrixIdentity();
	matBillboardY = XMMatrixIdentity();
	dirty = true;
}

Camera::~Camera()
{

}

void Camera::SetEye(XMFLOAT3 eye)
{
	this->eye = eye;

	dirty = true;
}

void Camera::SetTarget(XMFLOAT3 target)
{
	this->target = target;

	dirty = true;
}

void Camera::SetUp(XMFLOAT3 up)
{
	this->up = up;

	dirty = true;
}

void Camera::MoveCamera(XMFLOAT3 move)
{
	this->eye.x += move.x;
	this->eye.y += move.y;
	this->eye.z += move.z;
	this->target.x += move.x;
	this->target.y += move.y;
	this->target.z += move.z;

	dirty = true;
}

void Camera::Initialize(int window_width, int window_height)
{
	// �r���[�s��̐���
	Update();

	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void Camera::Update()
{
	if (dirty == true)
	{
		//���_
		XMVECTOR eyePosition = XMLoadFloat3(&eye);
		//�����_
		XMVECTOR targetPosition = XMLoadFloat3(&target);
		//�����
		XMVECTOR upVector = XMLoadFloat3(&up);

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
		matView = XMMatrixTranspose(matCameraRot);

		//���_���W��-1�����������W
		XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
		//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��
		XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition);
		XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition);
		XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition);
		//��̃x�N�g���ɂ܂Ƃ߂�
		XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

		//�r���[�s��ɕ��s�ړ�������ݒ�
		matView.r[3] = translation;

		//�r���{�[�h�s��
		matBillboard.r[0] = cameraAxisX;
		matBillboard.r[1] = cameraAxisY;
		matBillboard.r[2] = cameraAxisZ;
		matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

		//�J����X���AY���AZ��
		XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

		//X���͋���
		ybillCameraAxisX = cameraAxisX;
		//Y���̓��[���h���W�n��Y��
		ybillCameraAxisY = XMVector3Normalize(upVector);
		//Z����X����Y���̊O�ςŋ��߂�
		ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

		//Y�����r���{�[�h�s��
		matBillboardY.r[0] = ybillCameraAxisX;
		matBillboardY.r[1] = ybillCameraAxisY;
		matBillboardY.r[2] = ybillCameraAxisZ;
		matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);

		dirty = false;
	}
}