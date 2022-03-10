#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Audio.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Collision.h"
#include "Camera.h"
#include "DirectXCommon.h"

class GamePlayScene
{
public: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�������u����
	DirectXCommon* dx_cmd = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Camera* camera = nullptr;

private: //�����o�ϐ�
	//�f�o�b�O�e�L�X�g
	DebugText debugText;
	const int fontNumber = 0;

	//�p�[�e�B�N��
	ParticleManager* particle = nullptr;

	//�X�v���C�g
	Sprite* demo_back = nullptr;
	Sprite* demo_spr = nullptr;

	//�I�u�W�F�N�g
	Object3d* chr = nullptr;
	Object3d* obj = nullptr;

public: //�����o�֐�
	GamePlayScene();
	~GamePlayScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void DrawParticles();

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	/// <summary>
	/// �f�o�b�O�e�L�X�g�`��
	/// </summary>
	void DrawDebugText();

	/// <summary>
	/// �ϐ�������
	/// </summary>
	void ResetVariable();
};