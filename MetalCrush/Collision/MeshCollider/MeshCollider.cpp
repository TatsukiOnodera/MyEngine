#include "MeshCollider.h"
#include "Collision.h"
#include <assert.h>

using namespace DirectX;

MeshCollider::MeshCollider()
{
	m_shapeType = COLLISONSHAPE_MESH;
}

void MeshCollider::ConstructTriangles(Model* model)
{
	// ���ƕ��̍ő�l���擾
	positivePosition = model->GetPositivePos();
	negativePosition = model->GetNegativePos();

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
		for (uint32_t i = 0; i < triangleNum; i++)
		{
			// �O�p�`���쐬
			Triangle tri;
			uint32_t idx0 = indices[i * 3 + 0];
			uint32_t idx1 = indices[i * 3 + 1];
			uint32_t idx2 = indices[i * 3 + 2];

			tri.p0 = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, 1 };
			tri.p1 = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, 1 };
			tri.p2 = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, 1 };
			tri.ComputeNormal();

			// �Y���G���A�Ɋi�[
			SortOutTriangleArea(tri);
		}
	}
}

void MeshCollider::Update()
{
	m_invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

void MeshCollider::SortOutTriangleArea(const Triangle& tri)
{
	// ���̒���
	float width = positivePosition.x - negativePosition.x;
	// �c�̒���
	float height = positivePosition.z - negativePosition.z;
	// ���̃G���A����̒���
	float wDiv = width / c_maxAreaX;
	// �c�̃G���A����̒���
	float hDiv = height / c_maxAreaY;

	// ���̃G���A�̔ԍ�
	int wNum = static_cast<int>((tri.p0.m128_f32[0] - negativePosition.x) / wDiv);
	if (c_maxAreaX <= wNum)
	{
		wNum = c_maxAreaX - 1;
	}
	// �c�̃G���A�̔ԍ�
	int hNum = static_cast<int>((tri.p0.m128_f32[2] - negativePosition.z) / hDiv);
	if (c_maxAreaY <= hNum)
	{
		hNum = c_maxAreaY - 1;
	}

	// �Y���G���A�ɓ����
	m_area[hNum][wNum].triangle.emplace_back(tri);
}

std::vector<Triangle>  MeshCollider::GetPositionAreaNumber(const DirectX::XMVECTOR& pos)
{
	// ���̒���
	float width = positivePosition.x - negativePosition.x;
	// �c�̒���
	float height = positivePosition.z - negativePosition.z;
	// ���̃G���A����̒���
	float wDiv = width / c_maxAreaX;
	// �c�̃G���A����̒���
	float hDiv = height / c_maxAreaY;

	// ���̃G���A�̔ԍ�
	int wNum = static_cast<int>((pos.m128_f32[0] - negativePosition.x) / wDiv);
	if (c_maxAreaX <= wNum)
	{
		wNum = c_maxAreaX - 1;
	}
	// �c�̃G���A�̔ԍ�
	int hNum = static_cast<int>((pos.m128_f32[2] - negativePosition.z) / hDiv);
	if (c_maxAreaY <= hNum)
	{
		hNum = c_maxAreaY - 1;
	}

	return m_area[hNum][wNum].triangle;
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	Sphere local;
	local.center = XMVector3Transform(sphere.center, m_invMatWorld);
	local.radius *= XMVector3Length(m_invMatWorld.r[0]).m128_f32[0];

	std::vector<Triangle> triangles = GetPositionAreaNumber(local.center);
	std::vector<Triangle>::const_iterator it = triangles.cbegin();
	for (; it != triangles.cend(); ++it)
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

	for (int y = 0; y < c_maxAreaY; y++)
	{
		for (int x = 0; x < c_maxAreaX; x++)
		{
			std::vector<Triangle> triangles = m_area[y][x].triangle;
			std::vector<Triangle>::const_iterator it = triangles.cbegin();
			for (; it != triangles.cend(); ++it)
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
		}
	}

	return false;
}
