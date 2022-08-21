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
ID3D12Device* Object3d::s_dev = nullptr;
ID3D12GraphicsCommandList* Object3d::s_cmdList = nullptr;
Camera *Object3d::s_camera = nullptr;
Light* Object3d::s_light = nullptr;

Object3d::~Object3d()
{

}

bool Object3d::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	if (device == nullptr)
	{
		return false;
	}
	Object3d::s_dev = device;
	Object3d::s_camera = Camera::GetInstance();

	Model::StaticInitialize(s_dev);

	return true;
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//null�`�F�b�N
	assert(cmdList);
	Object3d::s_cmdList = cmdList;
}

void Object3d::PostDraw()
{
	s_cmdList = nullptr;
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

	// �萔�o�b�t�@B0�̐���
	result = s_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));

	m_name = typeid(*this).name();

	m_model->Initialize();
}

void Object3d::Update()
{
	if (m_dirty || s_camera->GetDirty() || s_light->GetDirty())
	{
		//���[���h�s��̍X�V
		if (m_isBillboard)
		{
			//�r���{�[�h�s��̍X�V
			m_matWorld = XMMatrixIdentity(); //�P�ʍs��
			//�g��s��
			m_matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
			//��]�s��
			m_matWorld *= XMMatrixRotationX(XMConvertToRadians(m_rotation.x));
			m_matWorld *= XMMatrixRotationY(XMConvertToRadians(m_rotation.y));
			m_matWorld *= XMMatrixRotationZ(XMConvertToRadians(m_rotation.z));
			//�r���{�[�h�s��
			m_matWorld *= s_camera->GetMatBillboard();
			//���s�ړ��s��
			m_matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		} 
		else
		{
			m_matWorld = XMMatrixIdentity(); //�P�ʍs��
			//�g��s��
			m_matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
			//��]�s��
			m_matWorld *= XMMatrixRotationX(XMConvertToRadians(m_rotation.x));
			m_matWorld *= XMMatrixRotationY(XMConvertToRadians(m_rotation.y));
			m_matWorld *= XMMatrixRotationZ(XMConvertToRadians(m_rotation.z));
			//���s�ړ��s��
			m_matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		}

		const XMMATRIX& matView = s_camera->GetMatView();
		const XMMATRIX& matProjection = s_camera->GetMatProject();

		///�萔�o�b�t�@�]��
		ConstBufferData* constMap = nullptr;
		HRESULT result = m_constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matView * matProjection;
			constMap->world = m_matWorld;
			constMap->cameraPos = s_camera->GetEye();
			constMap->color = m_color;
			m_constBuff->Unmap(0, nullptr);
		}

		//���f���f�[�^�X�V
		m_model->Update();

		if (m_collider)
		{
			m_collider->Update();
		}

		m_dirty = false;
	}
}

void Object3d::Draw()
{
	//���f�����Ȃ��Ȃ甲����
	if (m_model == nullptr)
	{
		return;
	}

	//�X�V
	Update();

	//NULL�`�F�b�N
	assert(s_cmdList);

	//�p�C�v���C���ƃ��[�g�V�O�l�`���̐ݒ�
	s_cmdList->SetPipelineState(m_model->GetPipelineState());
	s_cmdList->SetGraphicsRootSignature(m_model->GetRootSignature());
	//�v���~�e�B�u�`���ݒ�
	s_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//�萔�o�b�t�@���Z�b�g
	s_cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());

	//���C�g�̕`��
	s_light->Draw(s_cmdList, 2);

	//���f���`��
	m_model->Draw(s_cmdList);
}

void Object3d::SetPosition(XMFLOAT3 position)
{
	this->m_position = position;
	m_dirty = true;
}

void Object3d::SetRotation(XMFLOAT3 rotation)
{
	this->m_rotation = rotation;
	m_dirty = true;
}

void Object3d::SetScale(XMFLOAT3 scale)
{
	this->m_scale = scale;
	m_dirty = true;
}

void Object3d::SetColor(XMFLOAT4 color)
{
	this->m_color = color;
	m_dirty = true;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);

	this->m_collider.reset(collider);
}

void Object3d::SetShader(const int shaderType)
{
	m_model->SetGraphicsPipeline(shaderType);
}

void Object3d::SetSubTexture(const std::string& filename)
{
	m_model->SetSubTexture(filename);
}

void Object3d::SetMaskTexture(const std::string& filename)
{
	m_model->SetMaskTexture(filename);
}
