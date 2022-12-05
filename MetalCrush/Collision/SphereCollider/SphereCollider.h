#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: // �G�C���A�X
	using XMVECTOR = DirectX::XMVECTOR;

private: // �����o�ϐ�
	// ���S����̃I�t�Z�b�g
	XMVECTOR m_offset;
	// ���a
	float m_radius;

public: // �����o�֐�
	SphereCollider(XMVECTOR offset = { 0, 0, 0, 0 }, float radius = 1.0f)
	{
		m_offset = offset;
		m_radius = radius;

		// ����
		m_shapeType = COLLISONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

public: // �A�N�Z�b�T
	/// <summary>
	/// ���a���Z�b�g
	/// </summary>
	/// <param name="radius">���a</param>
	inline void SetRadius(float radius) { m_radius = radius; }
};
