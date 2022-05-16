#pragma once
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Audio.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Collision.h"
#include "Camera.h"
#include "DirectXCommon.h"
#include "BaseScene.h"
#include "Light.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <memory>

class GamePlayScene : public BaseScene
{
public: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�萔
	const int fontNumber = 0;

private: //�������u����
	//DIrectXCommon
	DirectXCommon* dx_cmd = nullptr;

	//����n
	Input* input = nullptr;

	//�I�[�f�B�I
	Audio* audio = nullptr;

	//�J����
	Camera* camera = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText debugText;

private: //�C���X�^���X
	//���C�g
	std::unique_ptr<Light> light = nullptr;

	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particle = nullptr;

	//�X�v���C�g
	std::unique_ptr<Sprite> demo_back = nullptr;

	//OBJ�I�u�W�F�N�g
	std::unique_ptr<Object3d> chr = nullptr;
	std::unique_ptr<Object3d> player = nullptr;
	std::unique_ptr<Object3d> wall[6];
	std::vector<std::unique_ptr<Object3d>> bullet;

	//FBX�I�u�W�F�N�g
	//std::unique_ptr<FbxObject3d> fbxObject = nullptr;

private: //�����o�ϐ�
	float frame = 0;

	float gravity = 0;

	float angleY = 0;
	float angleX = 0;

	enum WALLNUMBER
	{
		FRONT, BACK, RIGHT, LEFT, UP, DOWN, FIN,
	};

	struct BulletInfo
	{
		bool is_alive = false;

		XMFLOAT3 vec = { 0, 0, 0 };

		int deleteTimer = 0;
	};
	std::vector<std::unique_ptr<BulletInfo>> bulletInfo;

public: //�����o�֐�
	~GamePlayScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �ϐ�������
	/// </summary>
	void ResetVariable();

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��
	/// </summary>
	void AnyDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void DrawParticle(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �f�o�b�O�e�L�X�g�`��
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// A����B�ւ̃x�N�g���̐��������߂�
	/// </summary>
	/// <param name="objectA">�I�u�W�F�N�gA</param>
	/// <param name="objectB">�I�u�W�F�N�gB</param>
	/// <returns>�x�N�g���̐���</returns>
	XMFLOAT3 GetVectorAtoB(XMFLOAT3 objectA, XMFLOAT3 objectB);

	/// <summary>
	/// A����B�ւ̋��������߂�
	/// </summary>
	/// <param name="objectA">�I�u�W�F�N�gA</param>
	/// <param name="objectB">�I�u�W�F�N�gB</param>
	/// <returns>����</returns>
	float GetLength(XMFLOAT3 objectA, XMFLOAT3 objectB);
};