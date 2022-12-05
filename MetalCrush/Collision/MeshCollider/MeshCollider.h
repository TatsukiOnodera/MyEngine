#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���b�V���Փ˔���
/// </summary>
class MeshCollider : public BaseCollider
{
private: // �����o�ϐ�
	// �O�p�`�̔z��
	std::vector<Triangle> m_triangles;
	// ���[���h�s��̋t�s��
	DirectX::XMMATRIX m_invMatWorld = {};

public: // �����o�֐�
	MeshCollider()
	{
		m_shapeType = COLLISONSHAPE_MESH;
	}

	/// <summary>
	/// �O�p�`�̔z������
	/// </summary>
	/// <param name="model"></param>
	void ConstructTriangles(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="inter">��_</param>
	/// <returns>����</returns>
	bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="distance">����</param>
	/// <param name="inter">��_</param>
	/// <returns>����</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR *inter = nullptr);
};
