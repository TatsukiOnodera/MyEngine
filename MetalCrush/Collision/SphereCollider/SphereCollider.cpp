#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX& matWorld = m_object3d->GetMatWorld();

	// �����o�ϐ��X�V
	Sphere::center = matWorld.r[3] + m_offset;
	Sphere::radius = m_radius;
}