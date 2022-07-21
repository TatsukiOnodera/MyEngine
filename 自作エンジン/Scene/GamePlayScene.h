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
#include <array>

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
	//�f�o�b�O�e�L�X�g�p�e�N�X�`���̔ԍ�
	const int fontNumber = 0;
	//�V�F�[�_�[�̎��
	enum ShadersType
	{
		ADS, TOON, MONO, BLEND, SPECULAR
	};
	//�ǂ̔z�u
	enum WALLNUMBER
	{
		FRONT, BACK, RIGHT, LEFT, UP, DOWN, END
	};

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
	std::unique_ptr<Object3d> obj = nullptr;
	std::array<std::unique_ptr<Object3d>, END> defaultWall;
	std::vector<std::unique_ptr<Object3d>> bullet;
	std::unique_ptr<Object3d> enemy = nullptr;
	//FBX�I�u�W�F�N�g
	std::unique_ptr<FbxObject3d> fbxObject = nullptr;

private: //�����o�ϐ�
	//��������
	bool isDash;
	//���������l
	float add0;

	//Bullet�̐���
	std::vector<bool> bulletAlive;
	//BUllet�̃x�N�g��
	std::vector<XMFLOAT3> bulletVec;
	//�V���b�g�̊Ԋu
	int bulletTime;

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
	void ResetParameter();

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��
	/// </summary>
	void DrawObjects(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �G�t�F�N�g�`��
	/// </summary>
	void DrawEffect(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �f�o�b�O�e�L�X�g�`��
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);
};