#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
public: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

protected: //�萔
	//�e�N�X�`���̍ő喇��
	static const int spriteSRVCount = 512;
	//�e�N�X�`���̍ő喇��
	static const int vertNum = 4;

protected: //�ÓI�ϐ�
	//DirectXCommon->dev
	static ID3D12Device* dev;
	//DirectXCommon->cmdList
	static ID3D12GraphicsCommandList* cmdList; 
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	//�ˉe�s��
	static XMMATRIX matProjection;
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	static ComPtr<ID3D12DescriptorHeap> descHeap; 
	//�e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature; 

public: //�ÓI�֐�
	//�ÓI������
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�p�C�v���C������
	static void CreateGraphicsPipeline();

	//�X�v���C�g�̋��ʃf�[�^����
	static void CreateSpriteCommon(int window_width, int window_height);

	//�e�N�X�`���̃��[�h
	static void LoadTexture(UINT texNumber, const wchar_t* filename);

	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//�`��㏈��
	static void PostDraw();

	//�X�v���C�g�쐬
	static Sprite* CreateSprite(UINT texNumber, XMFLOAT2 anchorpoint = {0.0f, 0.0f});

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color; //�F

		XMMATRIX mat; //�s��
	};

	//�X�v���C�g�̒��_�f�[�^�^
	struct VertexPosUv
	{
		XMFLOAT3 pos; //xyz���W

		XMFLOAT2 uv; //uv���W
	};

protected: //�����o�ϐ�
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//���W
	XMFLOAT3 position = { 0, 0, 0 };
	//�F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//�傫��
	XMFLOAT2 size = { 10, 10 };
	//�e�N�X�`���̍�����W
	XMFLOAT2 texLeftTop = { 0, 0 };
	//�e�N�X�`���̐؂�o���T�C�Y
	XMFLOAT2 texSize = { 100, 100 };
	//�A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0.0f, 0.0f };
	//���[���h�s��
	XMMATRIX matWorld;
	//Z�����̉�]�p
	float rotation = 0.0f;
	//���E���]
	bool isFilpX = false;
	//�㉺���]
	bool isFilpY = false;
	//��\��
	bool isInvisible = false;

public: //�A�N�Z�b�T
	//���W
	XMFLOAT3 GetPostion() { return position; }
	void SetPosition(XMFLOAT3 position);

	//�F
	XMFLOAT4 GetColor() { return color; }
	void SetColor(XMFLOAT4 color);

	//�T�C�Y
	void SetSize(XMFLOAT2 size);

	//�`�揉�߂̍��W
	void SetLeftTop(XMFLOAT2 LeftTop);

	//�e�N�X�`���̃T�C�Y
	void SetTexSize(XMFLOAT2 texSize);

	//��]
	void SetRotation(float rotation);

	//���]
	void SetFilp(bool isFilpX, bool isFilpY);

	//��\��
	void SetInvisible(bool isInvisible) { this->isInvisible = isInvisible; }

public: //�����o�֐�
	//�R���X�g���N�^
	Sprite(XMFLOAT2 size, UINT texNumber, XMFLOAT2 anchorpoint);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//���_�f�[�^�X�V
	void SpriteTransferVertexBuffer();
};