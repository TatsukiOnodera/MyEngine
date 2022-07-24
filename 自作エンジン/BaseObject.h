#pragma once
#include <fbxsdk.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <SafeDelete.h>
#include <cassert>
#include <memory>
#include <array>
#include <vector>
#include "DirectXCommon.h"
#include "Input.h"
#include "InputList.h"
#include "Audio.h"
#include "Camera.h"
#include "DebugText.h"

using namespace DirectX;

class BaseObject : public InputList
{
public: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseObject() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;
};
