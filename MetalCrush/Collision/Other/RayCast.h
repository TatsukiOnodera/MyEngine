#pragma once
#include "BaseCollider.h"
#include <DirectXMath.h>

class Object3d;

/// <summary>
/// ���C�L���X�g�̏��
/// </summary>
struct RayCast
{
	// �Փˑ���̃I�u�W�F�N�g
	Object3d* m_object = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* m_collider = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR m_inter;
	// �Փ˓_�܂ł̋���
	float m_distance = 0.0f;
};
