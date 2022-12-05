#pragma once
#include "Input.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Audio.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Collision.h"
#include "Camera.h"
#include "DirectXCommon.h"
#include "BaseScene.h"
#include "LightGroup.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AutoLockOn.h"
#include "CollisionManager.h"
#include "MeshCollider.h"
#include "TouchableObject.h"

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
	std::unique_ptr<LightGroup> lightGroup = nullptr;

	// �p�[�e�B�N��
	std::unique_ptr<ParticleManager> explosion = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> reticle = nullptr;
	/*std::unique_ptr<Sprite> HP = nullptr;
	std::unique_ptr<Sprite> ammoNum[2];*/

	// �I�u�W�F�N�g
	std::unique_ptr<Model> desertModel;
	std::unique_ptr<Model> skyWallModel;
	std::unique_ptr<Model> playerModel;
	std::unique_ptr<Model> enemyModel;
	std::unique_ptr<Model> bulletModel;
	//std::unique_ptr<TouchableObject> desert = nullptr;
	std::unique_ptr<Object3d> desert = nullptr;
	std::unique_ptr<Object3d> skyWall = nullptr;
	std::vector<std::unique_ptr<Enemy>> enemy;
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Object3d> p = nullptr;

	// ���̑�
	std::unique_ptr<AutoLockOn> lockList = nullptr;
	CollisionManager* collisionManager = nullptr;

private: // �����o�ϐ�
	

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
	void DrawHUD(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �f�o�b�O�e�L�X�g�`��
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);

public: // �����o�֐�
	/// <summary>
	/// �Փ˔���
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// �v���C���[�ƃG�l�~�[�̏Փ˔���	
	/// </summary>
	void CheckPlayer2Enemy();

	/// <summary>
	/// �v���C���[�̒e�ƃG�l�~�[�̏Փ˔���
	/// </summary>
	void CheckPlayerBullets2Enemy();

	/// <summary>
	/// �v���C���[�ƃG�l�~�[�̒e�̏Փ˔���
	/// </summary>
	void CheckPlayer2EnemyBullets();

	/// <summary>
	/// �v���C���[�ƕǂ̏Փ˔���
	/// </summary>
	void CheckPlayer2Wall();
	
	/// <summary>
	/// �G�l�~�[�ƕǂ̏Փ˔���
	/// </summary>
	void CheckEnemy2Wall();

	/// <summary>
	/// 
	/// </summary>
	void CheckBullet2Wall();
};
