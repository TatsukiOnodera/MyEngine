#include "Sprite.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* Sprite::dev = nullptr;
UINT Sprite::descriptorHandleIncrementSize;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Sprite::rootsignature;
ComPtr<ID3D12PipelineState> Sprite::pipelinestate;
XMMATRIX Sprite::matProjection;
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap;
ComPtr<ID3D12Resource> Sprite::texBuff[spriteSRVCount];

bool Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	if (device == nullptr)
	{
		return false;
	}

	Sprite::dev = device;

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CreateGraphicsPipeline();

	CreateSpriteCommon(window_width, window_height);

	return true;
}

void Sprite::CreateGraphicsPipeline()
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/SpriteVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,

		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/SpritePS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // ��ɏ㏑�����[��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 ���W�X�^

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
}

void Sprite::CreateSpriteCommon(int window_width, int window_height)
{
	HRESULT result = S_FALSE;
	
	//���s�s��̎ˉe�s�񐶐�
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	//�f�X�N���v�^�q�[�v��ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	//�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	assert(SUCCEEDED(result));
}

void Sprite::LoadTexture(UINT texNumber, const wchar_t* filename)
{
	//�ُ�Ȕԍ��̎w������o
	assert(texNumber <= spriteSRVCount - 1);

	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadate{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename, //�摜
		WIC_FLAGS_NONE,
		&metadate, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadate.format,
		metadate.width,
		(UINT)metadate.height,
		(UINT16)metadate.arraySize,
		(UINT16)metadate.mipLevels);

	//�e�N�X�`���o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff[texNumber])
	);

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff[texNumber]->WriteToSubresource(
		0,
		nullptr, //�S�̈�ɃR�s�[
		img->pixels, //���f�[�^�A�h���X
		(UINT)img->rowPitch, //1���C���T�C�Y
		(UINT)img->slicePitch //�S���C���T�C�Y
	);

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = metadate.format; //RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //�QD�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v��texnumber�ԖڂɃV�F�[�_���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(
		texBuff[texNumber].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), texNumber, descriptorHandleIncrementSize)
	);
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	Sprite::cmdList = cmdList;

	//�p�C�v���C���ƃ��[�g�V�O�l�`���̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	cmdList->SetGraphicsRootSignature(rootsignature.Get());

	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw()
{
	cmdList = nullptr;
}

Sprite *Sprite::Create(UINT texNumber, XMFLOAT2 texLeftTop, XMFLOAT2 anchorpoint, bool isFilpX, bool isFilpY, bool isInvisible)
{
	XMFLOAT2 size = {10, 10};

	if (texBuff[texNumber])
	{
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		// �X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ɐݒ�
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	///�V�����X�v���C�g�����
	Sprite *sprite = new Sprite(size, texNumber, texLeftTop, anchorpoint, isFilpX, isFilpY, isInvisible);

	sprite->Initialize();

	sprite->Update();

	return sprite;
}

void Sprite::SetInvisible(bool isInvisible)
{
	this->isInvisible = isInvisible;
}

Sprite::Sprite(XMFLOAT2 size, UINT texNumber, XMFLOAT2 texLeftTop, XMFLOAT2 anchorpoint, bool isFilpX, bool isFilpY, bool isInvisible)
{
	this->size = size;
	this->texNumber = texNumber;
	this->texLeftTop = texLeftTop;
	this->anchorpoint = anchorpoint;
	this->texSize = size;
	this->isFilpX = isFilpX;
	this->isFilpY = isFilpY;
	this->isInvisible = isInvisible;
	this->matWorld = XMMatrixIdentity();
}

void Sprite::Initialize()
{
	HRESULT result = S_FALSE;

	//���_�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	//���_�o�b�t�@�̓]��
	SpriteTransferVertexBuffer();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * vertNum;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	//�萔�o�b�t�@�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->mat = matProjection;
	constBuff->Unmap(0, nullptr);
}

void Sprite::Update()
{
	//���[���h�s��̍X�V
	matWorld = XMMatrixIdentity();
	//Z����]
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	//���s�ړ�
	matWorld *= XMMatrixTranslation(position.x, position.y, 0);

	//�萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * matProjection;
	constMap->color = color;
	constBuff->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	//��\���t���O��true�Ȃ�
	if (isInvisible)
	{
		//�`�悹��������
		return;
	}

	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//���_�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			descriptorHandleIncrementSize));

	//�|���S���̕`��i�S���_�Ŏl�p�`�j
	cmdList->DrawInstanced(vertNum, 1, 0, 0);
}

void Sprite::SpriteTransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFilpX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFilpY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VertexPosUv vertices[vertNum];

	vertices[LB].pos = { left, bottom, 0.0f }; // ����
	vertices[LT].pos = { left, top, 0.0f }; // ����
	vertices[RB].pos = { right, bottom, 0.0f }; // �E��
	vertices[RT].pos = { right, top, 0.0f }; // �E��

	// �e�N�X�`�����擾
	if (texBuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // ����
		vertices[LT].uv = { tex_left,	tex_top }; // ����
		vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,	tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}

void Sprite::SetPosition(XMFLOAT2 position)
{
	this->position = position;

	Update();
}

void Sprite::SetLeftTop(XMFLOAT2 leftTop)
{
	this->texLeftTop = leftTop;

	SpriteTransferVertexBuffer();
}

void Sprite::SetSize(XMFLOAT2 size)
{
	this->size = size;

	SpriteTransferVertexBuffer();
}

void Sprite::SetTexSize(XMFLOAT2 texSize)
{
	this->texSize = texSize;

	SpriteTransferVertexBuffer();
}

void Sprite::SetColor(XMFLOAT4 color)
{
	this->color = color;

	Update();
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;

	Update();
}

void Sprite::SetFilp(bool isFilpX, bool isFilpY)
{
	this->isFilpX = isFilpX;
	this->isFilpY = isFilpY;

	SpriteTransferVertexBuffer();
}