#pragma once
#include "CollisionType.h"
#include "Object3d.h"

class BaseCollider
{
protected: //�����o�ϐ�
	//�I�u�W�F�N�g�i�[��
	Object3d* m_object3d = nullptr;
	//�`��^�C�v
	CollisionShapeType m_shapeType = SHAPE_UNKNOWN;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BaseCollider() = default;

	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~BaseCollider() = default;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info) { m_object3d->OnCollision(info); }

public: // �A�N�Z�b�T
	/// <summary>
	/// �I�u�W�F�N�g���Z�b�g
	/// </summary>
	/// <param name="object">�Z�b�g����I�u�W�F�N�g</param>
	inline void SetObject(Object3d* object) { m_object3d = object; }

	/// <summary>
	/// �I�u�W�F�N�g���擾
	/// </summary>
	/// <returns>�i�[�����I�u�W�F�N�g</returns>
	inline Object3d* GetObject3d() { return m_object3d; }

	/// <summary>
	/// �`��^�C�v�擾
	/// </summary>
	/// <returns>�`��^�C�v</returns>
	inline CollisionShapeType GetShapeType() { return m_shapeType; }
};