#include "PostEffect.h"
#include "WinApp.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() : Sprite({ 500.0f, 500.0f }, 100, { 0.0f, 0.0f })
{

}

void PostEffect::Initialize()
{
	HRESULT result;

	//���N���X�Ƃ��Ă̏�����
	Sprite::Initialize();

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WinApp::window_width,
		WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//�e�N�X�`����ԃN���A
	{
		//��f��
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		//��f����s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * WinApp::window_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//�f�X�N���v�^�q�[�v��ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srcDescHeapDesc{};
	srcDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srcDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srcDescHeapDesc.NumDescriptors = 1;
	//�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&srcDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //�QD�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v�ɃV�F�[�_���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(texBuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//��\���t���O��true�Ȃ�
	if (isInvisible)
	{
		//�`�悹��������
		return;
	}

	PreDraw(cmdList);

	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//���_�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	/*cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			descriptorHandleIncrementSize));*/
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�|���S���̕`��i�S���_�Ŏl�p�`�j
	cmdList->DrawInstanced(vertNum, 1, 0, 0);

	PostDraw();
}
