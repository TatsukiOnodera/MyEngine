#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX& matWorld = m_object3d->GetMatWorld();

	// メンバ変数更新
	Sphere::center = matWorld.r[3] + m_offset;
	Sphere::radius = m_radius;
}