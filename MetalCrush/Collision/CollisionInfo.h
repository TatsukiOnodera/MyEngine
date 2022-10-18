#pragma once
#include <DirectXMath.h>

class Object3d;
class BaseCollider;

struct CollisionInfo
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="object">�Փˑ���̃I�u�W�F�N�g</param>
	/// <param name="collider">�Փˑ���̃R���C�_�[</param>
	/// <param name="inter">�Փ˓_</param>
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::HXMVECTOR& inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

public: //�����o�ϐ�
	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓X
	DirectX::XMVECTOR inter;
};