#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���b�V���Փ˔���
/// </summary>
class MeshCollider : public BaseCollider
{
private: // �萔
	static const int c_maxAreaX = 16;
	static const int c_maxAreaY = c_maxAreaX;

private: // �T�u�N���X
	// �O�p�`�̏��
	struct AreaInfo
	{
		// �O�p�`�̏��
		std::vector<Triangle> triangle;

		// �f�X�g���N�^
		~AreaInfo()
		{
			triangle.clear();
		}
	};

private: // �����o�ϐ�
	// �O�p�`�̔z��
	AreaInfo m_area[c_maxAreaY][c_maxAreaX];
	// ���[���h�s��̋t�s��
	DirectX::XMMATRIX m_invMatWorld = {};
	// ���̍ő���W
	DirectX::XMFLOAT3 positivePosition = {};
	// ���̍ő���W
	DirectX::XMFLOAT3 negativePosition = {};

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MeshCollider();

	/// <summary>
	/// �O�p�`�̔z������
	/// </summary>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="div">�c���ɉ��������邩</param>
	void ConstructTriangles(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �O�p�`���G���A����
	/// </summary>
	/// <param name="tri">�O�p�`�̏��</param>
	/// <returns>�G���A�ԍ�</returns>
	void SortOutTriangleArea(const Triangle& tri);

	/// <summary>
	/// ���W���ǂ̃G���A�ɂ��邩
	/// </summary>
	/// <param name="pos">���W</param>
	/// <returns>�Y���̃G���A</returns>
	std::vector<Triangle> GetPositionAreaNumber(const DirectX::XMVECTOR& pos);

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
