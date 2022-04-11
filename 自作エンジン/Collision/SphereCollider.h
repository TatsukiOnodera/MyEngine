#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: //�G�C���A�X
	//DirectX���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;

private:
	//�I�u�W�F�N�g�̒��S����̃I�t�Z�b�g
	XMVECTOR offset;
	//���a
	float radius;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset">�I�t�Z�b�g</param>
	/// <param name="radius">���a</param>
	SphereCollider(XMVECTOR offset = { 0, 0, 0, 0 }, float radius = 1.0f) : offset(offset), radius(radius)
	{
		//���`����Z�b�g
		shapeType = COLLISONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���a�̃Z�b�g
	/// </summary>
	/// <param name="radius">���a</param>
	inline void SetRadius(float radius) { this->radius = radius; }
};
