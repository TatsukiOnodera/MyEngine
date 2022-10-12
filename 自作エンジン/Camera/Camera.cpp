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
	//ビルボード行列初期化
	m_matBillboard = XMMatrixIdentity();
	m_matBillboardY = XMMatrixIdentity();

	// ビュー行列の生成
	m_dirty = true;
	Update();

	// 透視投影による射影行列の生成
	UpdateMatProjection();
}

void Camera::Update()
{
	if (m_dirty == true)
	{
		//視点
		XMVECTOR eyePosition = XMLoadFloat3(&m_eye);
		//注視点
		XMVECTOR targetPosition = XMLoadFloat3(&m_target);
		//上方向
		XMVECTOR upVector = XMLoadFloat3(&m_up);

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
		m_matView = XMMatrixTranspose(matCameraRot);

		//視点座標に-1をかけた座標
		XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
		//カメラの位置からワールド原点へのベクトル
		XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition);
		XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition);
		XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition);
		//一つのベクトルにまとめる
		XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

		//ビュー行列に平行移動成分を設定
		m_matView.r[3] = translation;

		//ビルボード行列
		m_matBillboard.r[0] = cameraAxisX;
		m_matBillboard.r[1] = cameraAxisY;
		m_matBillboard.r[2] = cameraAxisZ;
		m_matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

		//カメラX軸、Y軸、Z軸
		XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

		//X軸は共通
		ybillCameraAxisX = cameraAxisX;
		//Y軸はワールド座標系のY軸
		ybillCameraAxisY = XMVector3Normalize(upVector);
		//Z軸はX軸→Y軸の外積で求める
		ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

		//Y軸回りビルボード行列
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
	// 最近値より最遠値が小さければ止める
	assert(m_nearZ < m_farZ);

	// 透視投影による射影行列の更新
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
	//注視点セット
	SetTarget(target);

	//オフセットベクトル
	XMVECTOR v0 = { eyeDistance.x, eyeDistance.y, eyeDistance.z, 0 };

	//回転行列
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(angleX));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleY));

	//注視点から始点へのベクトルを求める
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//注視点に足して視点を求める
	XMFLOAT3 eyePosition = { target.x + V.m128_f32[0], target.y + V.m128_f32[1], target.z + V.m128_f32[2] };

	//始点をセット
	SetEye(eyePosition);
}

XMFLOAT3 Camera::ConvertWindowYPos(const XMFLOAT3& pos, const XMFLOAT3& vec, const float& angleY)
{
	//移動ベクトル
	XMVECTOR v0 = { vec.x, vec.y, vec.z, 0 };

	//回転行列
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleY));

	//正面をもとに移動したベクトルの向きを出す
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//元の座標に移動したベクトルを足す
	XMFLOAT3 position = { pos.x + V.m128_f32[0], pos.y + V.m128_f32[1], pos.z + V.m128_f32[2] };

	return position;
}

XMFLOAT3 Camera::ConvertWindowXYPos(const XMFLOAT3& pos, const XMFLOAT3& vec, const float& angleX, const float& angleY)
{
	//移動ベクトル
	XMVECTOR v0 = { vec.x, vec.y, vec.z, 0 };

	//回転行列
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleX));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleY));

	//正面をもとに移動したベクトルの向きを出す
	XMVECTOR V = XMVector3TransformNormal(v0, rotM);

	//元の座標に移動したベクトルを足す
	XMFLOAT3 position = { pos.x + V.m128_f32[0], pos.y + V.m128_f32[1], pos.z + V.m128_f32[2] };

	return position;
}

XMFLOAT2 Camera::Convert3DPosTo2DPos(const XMFLOAT3& pos)
{
	// ビューポート行列（スクリーン行列）の作成
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
