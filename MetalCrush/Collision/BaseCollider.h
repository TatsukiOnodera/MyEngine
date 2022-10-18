#pragma once
#include "CollisionType.h"
#include "Object3d.h"

class BaseCollider
{
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
	/// �I�u�W�F�N�g���Z�b�g
	/// </summary>
	/// <param name="object">�Z�b�g����I�u�W�F�N�g</param>
	inline void SetObject(Object3d* object) { this->object3d = object; }

	/// <summary>
	/// �I�u�W�F�N�g���擾
	/// </summary>
	/// <returns>�i�[�����I�u�W�F�N�g</returns>
	inline Object3d* GetObject3d() { return object3d; }

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��^�C�v�擾
	/// </summary>
	/// <returns>�`��^�C�v</returns>
	inline CollisionShapeType GetShapeType() { return shapeType; }


	inline void OnCollision(const CollisionInfo& info) { object3d->OnCollision(info); }

protected: //�����o�ϐ�
	//�I�u�W�F�N�g�i�[��
	Object3d* object3d = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};