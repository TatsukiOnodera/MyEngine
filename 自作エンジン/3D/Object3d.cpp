#include "Object3d.h"
#include "BaseCollider.h"

#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

// �ÓI�����o�ϐ��̎���
ID3D12Device* Object3d::dev = nullptr;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
Camera *Object3d::camera = nullptr;
Light* Object3d::light = nullptr;

Object3d::~Object3d()
{
	if (collider)
	{
		delete collider;
	}
}

bool Object3d::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	if (device == nullptr)
	{
		return false;
	}
	Object3d::dev = device;
	Object3d::camera = Camera::GetInstance();

	Model::StaticInitialize(dev);

	return true;
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	//null�`�F�b�N
	assert(commandList);
	Object3d::cmdList = commandList;

	//�p�C�v���C���ƃ��[�g�V�O�l�`���̐ݒ�
	cmdList->SetPipelineState(graphicsPipeline->GetPipelineState());
	cmdList->SetGraphicsRootSignature(graphicsPipeline->GetRootSignature());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	cmdList = nullptr;
}

Object3d* Object3d::Create(const std::string& modelName, bool smooting)
{
	Object3d* object = new Object3d;

	Model* model = Model::CreateModel(modelName, smooting);

	object->SetModel(model);

	object->Initialize();

	object->Update();

	return object;
}

void Object3d::Initialize()
{
	HRESULT result;

	//�p�C�v���C������
	graphicsPipeline.reset(new PipelineManager(dev));
	assert(graphicsPipeline->GetPipelineState());
	assert(graphicsPipeline->GetRootSignature());

	// �萔�o�b�t�@B0�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	name = typeid(*this).name();

	model->Initialize();
}

void Object3d::Update()
{
	if (dirty == true || camera->GetDirty() == true || light->GetDirty() == true)
	{
		//���[���h�s��̍X�V
		if (isBillboard == true)
		{
			//�r���{�[�h�s��̍X�V
			matWorld = XMMatrixIdentity(); //�P�ʍs��
			//�g��s��
			matWorld *= XMMatrixScaling(scale.x, scale.y, scale.z);
			//��]�s��
			matWorld *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
			matWorld *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
			matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
			//�r���{�[�h�s��
			matWorld *= camera->GetMatBillboard();
			//���s�ړ��s��
			matWorld *= XMMatrixTranslation(position.x, position.y, position.z);
		} 
		else
		{
			matWorld = XMMatrixIdentity(); //�P�ʍs��
			//�g��s��
			matWorld *= XMMatrixScaling(scale.x, scale.y, scale.z);
			//��]�s��
			matWorld *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
			matWorld *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
			matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
			//���s�ړ��s��
			matWorld *= XMMatrixTranslation(position.x, position.y, position.z);
		}

		const XMMATRIX& matView = camera->GetMatView();
		const XMMATRIX& matProjection = camera->GetMatProject();

		///�萔�o�b�t�@�]��
		ConstBufferData* constMap = nullptr;
		HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matView * matProjection;
			constMap->world = matWorld;
			constMap->cameraPos = camera->GetEye();
			constMap->color = color;
			constBuff->Unmap(0, nullptr);
		}
		model->Update();

		if (collider)
		{
			collider->Update();
		}

		dirty = false;
	}
}

void Object3d::Draw(ID3D12GraphicsCommandList* commandList)
{
	//���f�����Ȃ��Ȃ甲����
	if (model == nullptr)
	{
		return;
	}

	//�X�V
	Update();

	//�`��O����
	PreDraw(commandList);
	
	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//���C�g�̕`��
	light->Draw(cmdList, 3);

	//���f���`��
	model->Draw(cmdList);

	//�`��㏈��
	PostDraw();
}

void Object3d::SetPosition(XMFLOAT3 position)
{
	this->position = position;
	dirty = true;
}

void Object3d::SetRotation(XMFLOAT3 rotation)
{
	this->rotation = rotation;
	dirty = true;
}

void Object3d::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
	dirty = true;
}

void Object3d::SetColor(XMFLOAT4 color)
{
	this->color = color;
	dirty = true;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);

	this->collider = collider;
}

void Object3d::SetGraphicsPipeline(const int shaderType)
{
	//null�`�F�b�N
	assert(graphicsPipeline);

	//�w��̃V�F�[�_�[����
	switch (shaderType)
	{
	case ADS:
		graphicsPipeline->CreateADSPipeline(dev);
		break;

	case TOON:
		graphicsPipeline->CreateToonPipeline(dev);
		break;

	default:
		assert(0);
	}
}
