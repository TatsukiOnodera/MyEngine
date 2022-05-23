#include "PostEffect.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() : Sprite({ 500.0f, 500.0f }, 100, { 0.0f, 0.0f })
{

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//非表示フラグがtrueなら
	if (isInvisible)
	{
		//描画せず抜ける
		return;
	}

	PreDraw(cmdList);

	//デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//頂点バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			descriptorHandleIncrementSize));

	//ポリゴンの描画（４頂点で四角形）
	cmdList->DrawInstanced(vertNum, 1, 0, 0);

	PostDraw();
}
