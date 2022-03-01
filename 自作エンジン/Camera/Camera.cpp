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
}

Camera::~Camera()
{

}

void Camera::MoveCamera(XMFLOAT3 move)
{
	this->eye.x += move.x;
	this->eye.y += move.y;
	this->eye.z += move.z;
	this->target.x += move.x;
	this->target.y += move.y;
	this->target.z += move.z;
}

void Camera::Initialize(int window_width, int window_height)
{
	// ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void Camera::Update()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}