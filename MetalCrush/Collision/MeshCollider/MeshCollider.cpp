#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;

void MeshCollider::ConstructTriangles(Model* model)
{
	// ���X�g���N���A
	m_triangles.clear();
	// ���b�V���z��擾
	const std::vector<std::unique_ptr<Mesh>>& meshes = model->GetMeshes();
	// �O�p�`�̊J�n�ԍ�
	int start = 0;
	std::vector<std::unique_ptr<Mesh>>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it)
	{
		Mesh* mesh = it->get();
		const std::vector<Mesh::VertexPosNormalUv>& vertices = mesh->GetVertices();
		const std::vector<uint32_t>& indices = mesh->GetIndices();

		// �O�p�`�̐�
		size_t triangleNum = indices.size() / 3;
		// ���̕������z��𑝂₷
		m_triangles.resize(m_triangles.size() + triangleNum);
		for (int i = 0; i < triangleNum; i++)
		{
			Triangle& tri = m_triangles[start + 1];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			tri.p0 = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, 1 };
			tri.p1 = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, 1 };
			tri.p2 = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, 1 };
			tri.ComputeNormal();
		}
		start += (int)triangleNum;
	}
}

void MeshCollider::Update()
{
	m_invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	Sphere local;
	local.center = XMVector3Transform(sphere.center, m_invMatWorld);
	local.radius *= XMVector3Length(m_invMatWorld.r[0]).m128_f32[0];

	std::vector<Triangle>::const_iterator it = m_triangles.cbegin();

	for (; it != m_triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		if (Collision::CheckSphere2Triangle(local, triangle, inter) == true)
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

	std::vector<Triangle>::const_iterator it = m_triangles.cbegin();

	for (; it != m_triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		XMVECTOR tempInter;
		if (Collision::CheckRay2Triangle(local, triangle, nullptr, &tempInter) == true)
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
