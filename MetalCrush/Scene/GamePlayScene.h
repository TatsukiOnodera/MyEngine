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

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <memory>
#include <array>

class GamePlayScene : public BaseScene
{
public: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �萔
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���̔ԍ�
	const int fontNumber = 0;

	// �V�F�[�_�[�̎��
	enum ShadersType
	{
		ADS, TOON, MONO, BLEND, SPECULAR
	};

	// �ǂ̔z�u
	enum WALLNUMBER
	{
		FRONT, BACK, RIGHT, LEFT, UP, DOWN, END
	};

private: // �������u����
	// DIrectX�R�}���h
	DirectXCommon* dx_cmd = nullptr;
	// ����n
	Input* input = nullptr;
	// �I�[�f�B�I
	//Audio* audio = nullptr;
	// �J����
	Camera* camera = nullptr;
	// �f�o�b�O�e�L�X�g
	DebugText debugText;

private: // �C���X�^���X
	// ���C�g
	std::unique_ptr<Light> light = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> backScreen = nullptr;
	std::unique_ptr<Sprite> reticle = nullptr;
	std::unique_ptr<Sprite> HP = nullptr;

	// OBJ�I�u�W�F�N�g
	std::unique_ptr<Object3d> ground = nullptr;
	std::vector<std::unique_ptr<Enemy>> enemy;

	// FBX�I�u�W�F�N�g
	std::unique_ptr<Player> player = nullptr;

private: // �����o�ϐ�
	// �W�I�̔ԍ�
	int targetNum = 0;

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GamePlayScene() override;

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
	void InitializeVariable();

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

	/// <summary>
	/// �n�_����I�_�ւ̋���
	/// </summary>
	/// <param name="pos1">�I�_</param>
	/// <param name="pos2">�n�_</param>
	/// <returns>��_�Ԃ̋���</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2 = { 0, 0, 0 });

	/// <summary>
	/// �Փ˔���
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// �v���C���[�ƃG�l�~�[�̏Փ˔���	
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	/// <param name="enemyPos">�G�l�~�[�̍��W</param>
	void CheckPlayer2Enemy(const XMFLOAT3& playerPos, const XMFLOAT3* enemyPos);

	/// <summary>
	/// �v���C���[�̒e�ƃG�l�~�[�̏Փ˔���
	/// </summary>
	/// <param name="playerBullets">�v���C���[�̒e</param>
	/// <param name="enemyPos">�G�l�~�[�̍��W</param>
	void CheckPlayerBullets2Enemy(const std::vector<std::unique_ptr<Bullet>>& playerBullets, const XMFLOAT3* enemyPos);

	/// <summary>
	/// �v���C���[�ƃG�l�~�[�̒e�̏Փ˔���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void CheckPlayer2EnemyBullets(const XMFLOAT3& playerPos);

	/// <summary>
	/// �v���C���[�ƕǂ̏Փ˔���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void CheckPlayer2Wall(XMFLOAT3& playerPos);
	
	/// <summary>
	/// �G�l�~�[�ƕǂ̏Փ˔���
	/// </summary>
	/// <param name="playerPos">�G�l�~�[�̍��W</param>
	void CheckEnemy2Wall(XMFLOAT3* enemyPos);

	/// <summary>
	/// �v���C���[�̒e�ƕǂ̏Փ˔���
	/// </summary>
	/// <param name="playerBullets">�v���C���[�̒e</param>
	void CheckPlayerBullets2Wall(const std::vector<std::unique_ptr<Bullet>>& playerBullets);
	
	/// <summary>
	/// �G�l�~�[�̒e�ƕǂ̏Փ˔���
	/// </summary>
	/// <param name="enemyBullets">�G�l�~�[�̒e</param>
	void CheckEnemyBullets2Wall();
};
