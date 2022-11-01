#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
public: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // �萔
	// �e�N�X�`���̍ő喇��
	static const int c_spriteSRVCount = 512;
	// �e�N�X�`���̍ő喇��
	static const int c_vertNum = 4;

protected: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color; // �F

		XMMATRIX mat; // �s��
	};

	// �X�v���C�g�̒��_�f�[�^�^
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W

		XMFLOAT2 uv; // uv���W
	};

protected: // �ÓI�ϐ�
	// �f�o�C�X
	static ID3D12Device* s_dev;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* s_cmdList; 
	// �f�X�N���v�^�T�C�Y
	static UINT s_descriptorHandleIncrementSize;
	// �ˉe�s��
	static XMMATRIX s_matProjection;
	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	static ComPtr<ID3D12DescriptorHeap> s_descHeap; 
	// �e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
	static ComPtr<ID3D12Resource> s_texBuff[c_spriteSRVCount];
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> s_pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> s_rootsignature; 

public: // �ÓI�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns>�������ۂ�</returns>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �X�v���C�g�̋��ʃf�[�^����
	/// </summary>
	static void CreateSpriteCommon();

	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���̊���U��ԍ�</param>
	/// <param name="filename">�t�@�C���̃p�X</param>
	static void LoadTexture(const UINT texNumber, const wchar_t* filename);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// �쐬
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���̔ԍ�</param>
	/// <param name="texLeftTop">����̍��W</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g�̍��W</param>
	/// <param name="isFilpX">�������ɔ��]</param>
	/// <param name="isFilpY">�c�����ɔ��]</param>
	/// <param name="isInvisible">�\�����邩</param>
	/// <returns>�X�v���C�g</returns>
	static Sprite* Create(const UINT texNumber, const XMFLOAT2& texLeftTop = { 0, 0 }, const XMFLOAT2& anchorpoint = { 0.0f, 0.0f }, const bool isFilpX = false, const bool isFilpY = false, const bool isInvisible = false);

protected: // �����o�ϐ�
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> m_vertBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW m_vbView;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> m_constBuff;
	// �e�N�X�`���ԍ�
	UINT m_texNumber = 0;
	// ���W
	XMFLOAT2 m_position = { 0, 0 };
	// �F
	XMFLOAT4 m_color = { 1, 1, 1, 1 };
	// �傫��
	XMFLOAT2 m_size = { 10, 10 };
	// �e�N�X�`���̍�����W
	XMFLOAT2 m_texLeftTop = { 0, 0 };
	// �e�N�X�`���̐؂�o���T�C�Y
	XMFLOAT2 m_texSize = { 100, 100 };
	// �A���J�[�|�C���g
	XMFLOAT2 m_anchorpoint = { 0.0f, 0.0f };
	// ���[���h�s��
	XMMATRIX m_matWorld;
	// Z�����̉�]�p
	float m_rotation = 0.0f;
	// ���E���]
	bool m_isFilpX = false;
	// �㉺���]
	bool m_isFilpY = false;
	// ��\��
	bool m_isInvisible = false;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	/// <param name="texNumber">�e�N�X�`���̔ԍ�</param>
	/// <param name="texLeftTop">����̍��W</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g�̍��W</param>
	/// <param name="isFilpX">�������ɔ��]</param>
	/// <param name="isFilpY">�c�����ɔ��]</param>
	/// <param name="isInvisible">�\�����邩</param>
	Sprite(const XMFLOAT2& size, const UINT texNumber, const XMFLOAT2& texLeftTop, const XMFLOAT2& anchorpoint, const bool isFilpX, const bool isFilpY, const bool isInvisible);
	
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
	
	/// <summary>
	/// ���_�f�[�^�X�V
	/// </summary>
	void SpriteTransferVertexBuffer();

public: // �A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	XMFLOAT2 GetPostion() { return m_position; }

	/// <summary>
	/// ���W���Z�b�g	
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT2 position)
	{
		m_position = position;

		Update();
	}

	/// <summary>
	/// �F���擾
	/// </summary>
	/// <returns>�F</returns>
	XMFLOAT4 GetColor() { return m_color; }

	/// <summary>
	/// �F���Z�b�g
	/// </summary>
	/// <param name="color">�F</param>
	void SetColor(XMFLOAT4 color)
	{
		m_color = color;

		Update();
	}

	/// <summary>
	/// �T�C�Y���擾
	/// </summary>
	/// <returns>�T�C�Y</returns>
	XMFLOAT2 GetSize() { return m_size; }

	/// <summary>
	/// �T�C�Y���Z�b�g
	/// </summary>
	/// <param name="size">�摜�T�C�Y</param>
	void SetSize(XMFLOAT2 size)
	{
		m_size = size;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// �e�N�X�`���̃T�C�Y���Z�b�g
	/// </summary>
	/// <param name="texSize">�e�N�X�`���̃T�C�Y</param>
	void SetTexSize(XMFLOAT2 texSize)
	{
		m_texSize = texSize;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// �摜�̍���̍��W���Z�b�g
	/// </summary>
	/// <param name="LeftTop">����̍��W</param>
	void SetLeftTop(XMFLOAT2 LeftTop)
	{
		m_texLeftTop = LeftTop;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// ��]�p���Z�b�g
	/// </summary>
	/// <param name="rotation">��]�p</param>
	void SetRotation(float rotation)
	{
		m_rotation = rotation;

		Update();
	}

	/// <summary>
	/// ���]�����Z�b�g
	/// </summary>
	/// <param name="isFilpX">X���𔽓]</param>
	/// <param name="isFilpY">Y���𔽓]</param>
	void SetFilp(bool isFilpX, bool isFilpY)
	{
		m_isFilpX = isFilpX;
		m_isFilpY = isFilpY;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// �s�������Z�b�g
	/// </summary>
	/// <param name="isInvisible">�s�����</param>
	void SetInvisible(bool isInvisible)
	{
		m_isInvisible = isInvisible;
	}
};