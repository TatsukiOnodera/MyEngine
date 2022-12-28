#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

#include <SafeDelete.h>
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
LightGroup* Object3d::s_lightGroup = nullptr;
std::unique_ptr<Model> Object3d::s_modelList[c_modelMaxCount];

Object3d::~Object3d()
{
	if (m_collider)
	{
		CollisionManager::GetInstance()->RemoveCollider(m_collider.get());
	}
}

bool Object3d::StaticInitialize(ID3D12Device* device)
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

	//�v���~�e�B�u�`���ݒ�
	s_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	s_cmdList = nullptr;
}

Model* Object3d::GetModel(const UINT modelNumber)
{
	assert(modelNumber <= c_modelMaxCount - 1);

	// ���ɂ���Ȃ�
	if (!(s_modelList[modelNumber]))
	{
		assert(0);
	}

	return s_modelList[modelNumber].get();
}

void Object3d::LoadModel(const UINT modelNumber, const std::string& modelName, bool smoothing)
{
	assert(modelNumber <= c_modelMaxCount - 1);

	// ���ɂ���Ȃ�
	if (s_modelList[modelNumber])
	{
		return;
	}

	// ���f���쐬
	s_modelList[modelNumber].reset(Model::Create(modelName, smoothing));
}

Object3d* Object3d::Create(const UINT modelNumber)
{
	// ���f�������邩
	if (!(s_modelList[modelNumber]))
	{
		assert(0);
	}

	Object3d* object = new Object3d;

	object->SetModel(s_modelList[modelNumber].get());

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
	
	// �N���X���擾
	name = typeid(*this).name();
}

void Object3d::Update()
{
	if (m_dirty == true || s_camera->GetDirty() == true)
	{
		// �s��̍X�V
		UpdateWorldMatrix();

		// �r���[�s��
		const XMMATRIX& matView = s_camera->GetMatView();
		// �ˉe�s��
		const XMMATRIX& matProjection = s_camera->GetMatProject();

		///�萔�o�b�t�@�]��
		ConstBufferData* constMap = nullptr;
		HRESULT result = m_constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matView * matProjection;
			// �e������Ȃ�s���������
			if (m_parent)
			{
				m_matWorld = m_matWorld * m_parent->GetMatWorld();
			}
			constMap->world = m_matWorld;
			constMap->cameraPos = s_camera->GetEye();
			constMap->color = m_color;
			constMap->shininess = m_shininess;
			constMap->tiling = m_tiling;
			constMap->offset = m_offset;
			m_constBuff->Unmap(0, nullptr);
		}

		// �R���C�_�[�̍X�V
		if (m_collider)
		{
			m_collider->Update();
		}

		m_dirty = false;
	}
}

void Object3d::UpdateWorldMatrix()
{
	// ���[���h�s��̍X�V
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
		m_matWorld *= m_isBillboard;
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
}

void Object3d::Draw()
{
	//���f�����Ȃ�����ʊO�Ȃ甲����
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
	
	//�萔�o�b�t�@���Z�b�g
	s_cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());

	//���C�g�̕`��
	s_lightGroup->Draw(s_cmdList, 2);

	//���f���`��
	m_model->Draw(s_cmdList);
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->m_collider.reset(collider);

	CollisionManager::GetInstance()->AddCollider(collider);
	m_collider->Update();
}

void Object3d::OnCollision(const CollisionInfo& info)
{

}
