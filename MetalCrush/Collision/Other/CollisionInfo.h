#pragma once
#include <DirectXMath.h>

class Object3d;
class BaseCollider;

/// <summary>
/// �Փˏ��
/// </summary>
struct CollisionInfo
{
public: //�����o�ϐ�
	//�Փˑ���̃I�u�W�F�N�g
	Object3d* m_object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* m_collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR m_inter;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="object">�Փˑ���̃I�u�W�F�N�g</param>
	/// <param name="collider">�Փˑ���̃R���C�_�[</param>
	/// <param name="inter">�Փ˓_</param>
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::HXMVECTOR& inter)
	{
		m_object = object;
		m_collider = collider;
		m_inter = inter;
	}
};