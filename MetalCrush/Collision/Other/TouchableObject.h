#pragma once
#include "Object3d.h"

/// <summary>
/// �ڐG�\�I�u�W�F�N�g
/// </summary>
class TouchableObject : public Object3d
{
public: // �����o�֐�
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="div">�c�����������ɂ��邩</param>
	/// <returns>�ڐG�\�I�u�W�F�N�g</returns>
	static TouchableObject* Create(Model* model = nullptr);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f���f�[�^</param>
	/// <returns>����</returns>
	bool Initialize(Model* model);
};
