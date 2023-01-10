#include "DirectXCommon.h"

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon dx_cmd;

	return &dx_cmd;
}

DirectXCommon::DirectXCommon()
{

}

DirectXCommon::~DirectXCommon()
{
	// �J���R��̊m�F
	ID3D12DebugDevice* debugInterface;
	if (SUCCEEDED(m_dev.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}
}

bool DirectXCommon::Initialize(WinApp* app)
{
	OnDebugLayer();

	if (!(CreateDevice()))
	{
		return false;
	}
	if (!(CreateCmdList()))
	{
		return false;
	}
	if (!(CreateDoubleBuffer(app)))
	{
		return false;
	}
	if (!(CreateFence()))
	{
		return false;
	}
	if (!(CreateDepthBuffer()))
	{
		return false;
	}

	return true;
}

void DirectXCommon::OnDebugLayer()
{
	// �f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void DirectXCommon::PreDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex;
	bbIndex = m_swapchain->GetCurrentBackBufferIndex();

	// �\����Ԃ���`���ԂɕύX
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// �`���w��
	// �����_�\�^�[�Q�b�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		m_dev->GetDescriptorHandleIncrementSize(m_descHeap.Type)
	);

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// �[�x�o�b�t�@�̃N���A
	ClearDepth();

	// ��ʃN���A
	float R = 86;
	float G= 119;
	float B = 252;
	float A = 255;
	float clearColor[] = { powf((R / 255), 2.2f), powf((G / 255), 2.2f), powf((B / 255), 2.2f), A / 255 };// ���ۂ��F
	m_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	// �r���[�|�[�g�̈�̐ݒ�
	m_cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));
	// �V�U�[��`�̐ݒ�
	m_cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));
}

void DirectXCommon::PostDraw()
{
	// ���\�[�X�o���A��ύX
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// ���߂̃N���[�Y
	m_cmdList->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { m_cmdList.Get() }; // �R�}���h���X�g�̔z��
	m_cmdQueue->ExecuteCommandLists(1, cmdLists);

	// �o�b�t�@���t���b�v�i���\�̓��ւ��j
	m_swapchain->Present(1, 0);

	// �R�}���h�L���[�̎��s������҂�
	m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
	if (m_fence->GetCompletedValue() != m_fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_fence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	m_cmdAllocator->Reset(); // �L���[���N���A
	m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
}

bool DirectXCommon::CreateDevice()
{
	HRESULT result = S_FALSE;

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	if (result == S_FALSE)
	{
		return false;
	}

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	for (int i = 0; m_dxgiFactory->EnumAdapters1(i, &m_tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(m_tmpAdapter);
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); // �A�_�v�^�[�̏����擾

		// �\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description; // �A�_�v�^�[��
		// inetl UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			m_tmpAdapter = adapters[i];   // �̗p
			break;
		}
	}

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (int i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(m_tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&m_dev));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
	if (result == S_FALSE)
	{
		return false;
	}

	return true;
}

bool DirectXCommon::CreateCmdList()
{
	HRESULT result = S_FALSE;

	// �R�}���h�A���P�[�^�𐶐�
	result = m_dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_cmdAllocator));
	if (result == S_FALSE)
	{
		return false;
	}

	// �R�}���h���X�g�𐶐�
	result = m_dev->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_cmdList));
	if (result == S_FALSE)
	{
		return false;
	}

	// �W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	m_dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));

	return true;
}

bool DirectXCommon::CreateDoubleBuffer(WinApp* app)
{
	HRESULT result = S_FALSE;

	// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	ComPtr<IDXGISwapChain1> swapchain1;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �F���̏���
	swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2;  // �o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	m_dxgiFactory->CreateSwapChainForHwnd(
		m_cmdQueue.Get(),
		app->GetHWND(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	swapchain1.As(&m_swapchain);

	// �e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
	m_descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	m_descHeap.NumDescriptors = 2;    // ���\�̂Q��
	m_dev->CreateDescriptorHeap(&m_descHeap,
		IID_PPV_ARGS(&m_rtvHeaps));

	// ���\�̂Q���ɂ���
	m_backBuffers.resize(m_descHeap.NumDescriptors);
	for (int i = 0; i < 2; i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(), //�q�[�v�̐擪�A�h���X
				i, //�f�B�X�N���v�^�̔ԍ�
				m_dev->GetDescriptorHandleIncrementSize(m_descHeap.Type)); //�f�B�X�N���v�^����̃T�C�Y

			// �����_�[�^�[�Q�b�g�r���[�̐���
		m_dev->CreateRenderTargetView(
			m_backBuffers[i].Get(),
			nullptr,
			handle);
		m_dev->CreateRenderTargetView(
			m_backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				m_dev->GetDescriptorHandleIncrementSize(m_descHeap.Type)
			)
		);
	}
	if (result == S_FALSE)
	{
		return false;
	}

	return true;
}

bool DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;

	// �t�F���X�̐���
	result = m_dev->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_fence));
	if (result == S_FALSE)
	{
		return false;
	}

	return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// �[�x�o�b�t�@
	// �[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32_TYPELESS,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// �[�x�o�b�t�@�̐���
	result = m_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�̏������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_depthBuffer));
	if (result == S_FALSE)
	{
		return false;
	}

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	result = m_dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));
	if (result == S_FALSE)
	{
		return false;
	}

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_dev->CreateDepthStencilView(
		m_depthBuffer.Get(),
		&dsvDesc,
		m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	// �[�x�l�e�N�X�`���p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dstHeapDesc = {};
	dstHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dstHeapDesc.NodeMask = 0;
	dstHeapDesc.NumDescriptors = 1;
	dstHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = m_dev->CreateDescriptorHeap(&dstHeapDesc, IID_PPV_ARGS(&m_depthSRVHeap));
	if (result == S_FALSE)
	{
		return false;
	}

	// �[�x�l�e�N�X�`���r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC dstDesc = {};
	dstDesc.Format = DXGI_FORMAT_R32_FLOAT;
	dstDesc.Texture2D.MipLevels = 1;
	dstDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	dstDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_dev->CreateShaderResourceView(
		m_depthBuffer.Get(),
		&dstDesc,
		m_depthSRVHeap->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}

void DirectXCommon::ClearDepth()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();		
	// �[�x�o�b�t�@�̃N���A
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}