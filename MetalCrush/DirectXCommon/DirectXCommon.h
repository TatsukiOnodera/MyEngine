#pragma once
#include "WinApp.h"

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <vector>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

/// <summary>
/// DirectX���
/// </summary>
class DirectXCommon
{
public: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static DirectXCommon* GetInstance();

private: // �����o�ϐ�
	// �f�o�C�X
	ComPtr<ID3D12Device> m_dev;
	// DXGI�t�@�N�g���[
	ComPtr<IDXGIFactory6> m_dxgiFactory;
	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> m_swapchain;
	// �R�}���h�A���P�[�^�[
	ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
	// �R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;
	// �R�}���h�L���[
	ComPtr<ID3D12CommandQueue> m_cmdQueue;
	// �����_�[�^�[�Q�b�g�r���[�p�̃f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_rtvHeaps;
	// �A�_�v�^�[�I�u�W�F�N�g
	ComPtr<IDXGIAdapter1> m_tmpAdapter;
	// �f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC m_descHeap{};
	// �t�F���X
	ComPtr<ID3D12Fence> m_fence;
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> m_depthBuffer;
	// �o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> m_backBuffers;
	// �t�F���X�p�̒l
	UINT64 m_fenceVal = 0;
	// �[�x�r���[�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	// �[�x�l�e�N�X�`���p
	ComPtr<ID3D12DescriptorHeap> m_depthSRVHeap;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DirectXCommon();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DirectXCommon();

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(WinApp* app);

	/// <summary>
	/// �k�x�o�b�t�@�̃N���A
	/// </summary>
	void ClearDepth();

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();

private: //�����o�֐�
	/// <summary>
	/// �f�o�b�O���C���[���I���ɂ���
	/// </summary>
	void OnDebugLayer();

	/// <summary>
	/// �f�o�C�X���쐬
	/// </summary>
	/// <returns>�L��</returns>
	bool CreateDevice();

	/// <summary>
	/// �R�}���h���X�g���쐬
	/// </summary>
	/// <returns>�L��</returns>
	bool CreateCmdList();

	/// <summary>
	/// �_�u���o�b�t�@�̍쐬
	/// </summary>
	/// <returns>�L��</returns>
	bool CreateDoubleBuffer(WinApp* app);

	/// <summary>
	/// �t�F���X�̍쐬
	/// </summary>
	/// <returns>�L��</returns>
	bool CreateFence();

	/// <summary>
	/// �[�x�o�b�t�@�̍쐬
	/// </summary>
	/// <returns>�L��</returns>
	bool CreateDepthBuffer();

public: // �A�N�Z�b�T
	/// <summary>
	/// �f�o�C�X�擾
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	ID3D12Device* GetDev() { return m_dev.Get(); }

	/// <summary>
	/// �R�}���h���X�g�擾
	/// </summary>
	/// <returns>�R�}���h���X�g</returns>
	ID3D12GraphicsCommandList* GetCmdList() { return m_cmdList.Get(); }



	/// <summary>
	/// �[�x�l�e�N�X�`���p�V�F�[�_�[���\�[�X�r���[��GPU�n���h���擾
	/// </summary>
	/// <returns>GPU�n���h��</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDepthSRVHeapGPUHandle() { return m_depthSRVHeap->GetGPUDescriptorHandleForHeapStart(); }
};