#include "MeshCollider.h"
#include "Collision.h"
#include <assert.h>

using namespace DirectX;

void MeshCollider::ConstructTriangles(Model* model, int div)
{
	// ���ƕ��̍ő�l���擾
	positivePosition = model->GetPositivePos();
	negativePosition = model->GetNegativePos();

	// ���������L�^
	m_division = div;

	// ���X�g���N���A
	m_triangles.clear();
	// ���b�V���z��擾
	const std::vector<std::unique_ptr<Mesh>>& meshes = model->GetMeshes();
	// �O�p�`�̊J�n�ԍ�
	uint32_t start = 0;
	std::vector<std::unique_ptr<Mesh>>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it)
	{
		Mesh* mesh = it->get();
		const std::vector<Mesh::VertexPosNormalUv>& vertices = mesh->GetVertices();
		const std::vector<uint32_t>& indices = mesh->GetIndices();

		// �O�p�`�̐�
		uint32_t triangleNum = static_cast<uint32_t>(indices.size()) / 3;
		// ���̕������z��𑝂₷
		m_triangles.resize(m_triangles.size() + triangleNum);
		for (uint32_t i = 0; i < triangleNum; i++)
		{
			TriangleInfo& tri = m_triangles[start + 1];
			uint32_t idx0 = indices[i * 3 + 0];
			uint32_t idx1 = indices[i * 3 + 1];
			uint32_t idx2 = indices[i * 3 + 2];

			tri.areaNumber = SetTriangleAreaNumber(m_triangles[start + 1].triangle);

			tri.triangle.p0 = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, 1 };
			tri.triangle.p1 = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, 1 };
			tri.triangle.p2 = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, 1 };
			tri.triangle.ComputeNormal();
		}
		start += (uint32_t)triangleNum;
	}

	int c = 0;
}

void MeshCollider::Update()
{
	m_invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

int MeshCollider::SetTriangleAreaNumber(const Triangle& tri)
{
	if (m_division == 0)
	{
		assert(0);
	}

	// ���̒���
	float width = positivePosition.x - negativePosition.x;
	// �c�̒���
	float height = positivePosition.z - negativePosition.z;
	// ���̃G���A����̒���
	float wDiv = width / m_division;
	// �c�̃G���A����̒���
	float hDiv = height / m_division;

	// ���̃G���A�̔ԍ�
	int wNum = static_cast<int>((tri.p0.m128_f32[0] - negativePosition.x) / wDiv);
	// �c�̃G���A�̔ԍ�
	int hNum = static_cast<int>((tri.p0.m128_f32[2] - negativePosition.z) / hDiv);

	return wNum + hNum * m_division;
}

int MeshCollider::SetPositionAreaNumber(const DirectX::XMVECTOR& pos)
{
	if (m_division == 0)
	{
		assert(0);
	}

	// ���̒���
	float width = positivePosition.x - negativePosition.x;
	// �c�̒���
	float height = positivePosition.z - negativePosition.z;
	// ���̃G���A����̒���
	float wDiv = width / m_division;
	// �c�̃G���A����̒���
	float hDiv = height / m_division;

	// ���̃G���A�̔ԍ�
	int wNum = static_cast<int>((pos.m128_f32[0] - negativePosition.x) / wDiv);
	// �c�̃G���A�̔ԍ�
	int hNum = static_cast<int>((pos.m128_f32[2] - negativePosition.z) / hDiv);

	return wNum + hNum * m_division;
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	Sphere local;
	local.center = XMVector3Transform(sphere.center, m_invMatWorld);
	local.radius *= XMVector3Length(m_invMatWorld.r[0]).m128_f32[0];
	int localNum = SetPositionAreaNumber(sphere.center);

	std::vector<TriangleInfo>::const_iterator it = m_triangles.cbegin();
	for (; it != m_triangles.cend(); ++it)
	{
		if (it->areaNumber == localNum)
		{
			continue;
		}

		const TriangleInfo& triangleInfo = *it;
		if (Collision::CheckSphere2Triangle(local, triangleInfo.triangle, inter) == true)
		{
			if (inter)
			{
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				// ���[���h���W�̌�_
				*inter = XMVector3Transform(*inter, matWorld);
			}

			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter)
{
	Ray local;
	local.start = XMVector3Transform(ray.start, m_invMatWorld);
	local.dir = XMVector3TransformNormal(ray.dir, m_invMatWorld);

	std::vector<TriangleInfo>::const_iterator it = m_triangles.cbegin();

	for (; it != m_triangles.cend(); ++it)
	{
		const TriangleInfo& triangleInfo = *it;
		XMVECTOR tempInter;
		if (Collision::CheckRay2Triangle(local, triangleInfo.triangle, nullptr, &tempInter) == true)
		{
			const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
			// ���[���h���W�̌�_
			tempInter = XMVector3Transform(tempInter, matWorld);

			if (distance)
			{
				XMVECTOR sub = tempInter - ray.start;
				*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];
			}
			if (inter)
			{
				*inter = tempInter;
			}

			return true;
		}
	}

	return false;
}
