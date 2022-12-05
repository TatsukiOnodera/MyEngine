#pragma once
#include "CollisionPrimitive.h"
#include "RayCast.h"
#include <d3d12.h>
#include <memory>
#include <forward_list>

class BaseCollider;

/// <summary>
/// �Փ˃}�l�[�W��
/// </summary>
class CollisionManager
{
public: // �ÓI�����o�֐�
	static CollisionManager* GetInstance();

private: // �����o�ϐ�
	std::forward_list<BaseCollider*> m_colliders;

private: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CollisionManager() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CollisionManager() = default;

	/// <summary>
	/// �R�s�[�C���X�g���N�^
	/// </summary>
	/// <param name="">�C���X�g���N�^</param>
	CollisionManager(const CollisionManager&) = delete;

	/// <summary>
	/// �R�s�[������Z�q
	/// </summary>
	/// <param name="">�C���X�g���N�^</param>
	/// <returns>�Փ˃}�l�[�W��</returns>
	CollisionManager& operator=(const CollisionManager&) = delete;

public: // �����o�֐�
	/// <summary>
	/// �R���C�_�[�ǉ�
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider* collider)
	{
		m_colliders.emplace_front(collider);
	}

	/// <summary>
	/// �R���C�_�[�폜
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider* collider)
	{
		m_colliders.remove(collider);
	}

	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="hitInfo">�Փˏ��</param>
	/// <param name="maxDistance">�ő勗��</param>
	/// <returns>����</returns>
	bool RayCastHit(const Ray& ray, RayCast* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// ���ׂĂ̏Փ˃`�F�b�N
	/// </summary>
	void CheckAllCollisions();
};
