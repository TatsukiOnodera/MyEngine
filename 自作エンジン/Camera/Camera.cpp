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
	// ビュー行列の生成
	Update();

	// 透視投影による射影行列の生成
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
		//視点
		XMVECTOR eyePosition = XMLoadFloat3(&eye);
		//注視点
		XMVECTOR targetPosition = XMLoadFloat3(&target);
		//上方向
		XMVECTOR upVector = XMLoadFloat3(&up);

		//カメラZ軸
		XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
		//ベクトルを正規化
		cameraAxisZ = XMVector3Normalize(cameraAxisZ);

		//0ベクトルだと向きが定まらないので除外
		assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
		assert(!XMVector3IsInfinite(cameraAxisZ));
		assert(!XMVector3Equal(upVector, XMVectorZero()));
		assert(!XMVector3IsInfinite(upVector));

		//カメラのX軸
		XMVECTOR cameraAxisX;
		//X軸は上方向→Z軸の外積で求める
		cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
		//ベクトルを正規化
		cameraAxisX = XMVector3Normalize(cameraAxisX);

		//カメラのY軸
		XMVECTOR cameraAxisY;
		//Y軸はZ軸→X軸の外積で求める
		cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
		//ベクトルを正規化
		cameraAxisY = XMVector3Normalize(cameraAxisY);

		//カメラ回転行列
		XMMATRIX matCameraRot;
		//カメラ座標系→ワールド座標系の変換行列
		matCameraRot.r[0] = cameraAxisX;
		matCameraRot.r[1] = cameraAxisY;
		matCameraRot.r[2] = cameraAxisZ;
		matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

		//転置により逆行列を計算
		matView = XMMatrixTranspose(matCameraRot);

		//視点座標に-1をかけた座標
		XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
		//カメラの位置からワールド原点へのベクトル
		XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition);
		XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition);
		XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition);
		//一つのベクトルにまとめる
		XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

		//ビュー行列に平行移動成分を設定
		matView.r[3] = translation;

		//ビルボード行列
		matBillboard.r[0] = cameraAxisX;
		matBillboard.r[1] = cameraAxisY;
		matBillboard.r[2] = cameraAxisZ;
		matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

		//カメラX軸、Y軸、Z軸
		XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

		//X軸は共通
		ybillCameraAxisX = cameraAxisX;
		//Y軸はワールド座標系のY軸
		ybillCameraAxisY = XMVector3Normalize(upVector);
		//Z軸はX軸→Y軸の外積で求める
		ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

		//Y軸回りビルボード行列
		matBillboardY.r[0] = ybillCameraAxisX;
		matBillboardY.r[1] = ybillCameraAxisY;
		matBillboardY.r[2] = ybillCameraAxisZ;
		matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);

		dirty = false;
	}
}