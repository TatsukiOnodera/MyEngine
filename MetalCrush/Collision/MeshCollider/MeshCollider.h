#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���b�V���Փ˔���
/// </summary>
class MeshCollider : public BaseCollider
{
private: // �T�u�N���X
	// �O�p�`�̏��
	struct TriangleInfo
	{
		// �O�p�`�̏��
		Triangle triangle;
		// �G���A�̃i���o�[
		unsigned long int areaNumber = 0;
	};

private: // �����o�ϐ�
	// �O�p�`�̔z��
	std::vector<TriangleInfo> m_triangles;
	// ���[���h�s��̋t�s��
	DirectX::XMMATRIX m_invMatWorld = {};
	// ���̍ő���W
	DirectX::XMFLOAT3 positivePosition = {};
	// ���̍ő���W
	DirectX::XMFLOAT3 negativePosition = {};
	// �c���ɉ��������邩
	 int m_division = 0;

public: // �����o�֐�
	MeshCollider()
	{
		m_shapeType = COLLISONSHAPE_MESH;
	}

	/// <summary>
	/// �O�p�`�̔z������
	/// </summary>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="div">�c���ɉ��������邩</param>
	void ConstructTriangles(Model* model, int div);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �O�p�`���G���A����
	/// </summary>
	/// <param name="tri">�O�p�`�̏��</param>
	/// <returns>�G���A�ԍ�</returns>
	int SetTriangleAreaNumber(const Triangle& tri);

	/// <summary>
	/// ���W���G���A����
	/// </summary>
	/// <param name="pos">���W</param>
	/// <returns>�G���A�ԍ�</returns>
	int SetPositionAreaNumber(const DirectX::XMVECTOR& pos);

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
