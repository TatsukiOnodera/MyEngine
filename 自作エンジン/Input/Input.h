#pragma once
#include "InputList.h"
#include "WinApp.h"
#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>
#include <Xinput.h>

class Input
{
public: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

private: // �����o�ϐ�
	// Direct Input�̃o�[�W�����w��
	constexpr static int s_dInputVersion = 0x0800;
	// DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> dInput;
	// �L�[�{�[�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devKeyboard;
	// �L�[����
	BYTE keys[256] = {};
	// �O�t���[���̃L�[���
	BYTE oldKeys[256] = {};
	// �}�E�X�f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;
	// �}�E�X����
	DIMOUSESTATE2 mouseState = {};
	// �O�t���[���̃}�E�X����
	DIMOUSESTATE2 oldMouseState = {};
	// �Q�[���p�b�h�̔���
	XINPUT_STATE gamePadState = {};
	// �O�t���[���̃Q�[���p�b�h�̔���
	XINPUT_STATE oldGamePadState = {};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Input* GetInstance();

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* win);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[����
	/// </summary>
	/// <param name="key">�����Ă���L�[</param>
	/// <returns>�����Ă��邩</returns>
	bool PushKey(BYTE key);

	/// <summary>
	/// �L�[���́i�������s�j
	/// </summary>
	/// <param name="key">�����Ă���L�[</param>
	/// <returns>�����Ă��邩</returns>
	bool TriggerKey(BYTE key);

	/// <summary>
	/// ���N���b�N
	/// </summary>
	/// <param name="Mouse">�����Ă���{�^��</param>
	/// <returns>�����Ă��邩</returns>
	bool PushMouse(const int Mouse);

	/// <summary>
	/// �E�N���b�N
	/// </summary>
	/// <param name="Mouse">�����Ă���{�^��</param>
	/// <returns>�����Ă��邩</returns>
	bool TriggerMouse(const int Mouse);
	
	/// <summary>
	/// �Q�[���p�b�h���X�e�B�b�N��|��������
	/// </summary>
	/// <returns>�X�e�B�b�N��|��������</returns>
	XMFLOAT2 LeftStickAngle();

	/// <summary>
	/// �Q�[���p�b�h���X�e�B�b�N��|��������
	/// </summary>
	/// <returns>�X�e�B�b�N��|��������</returns>
	XMFLOAT2 RightStickAngle();

	/// <summary>
	/// �Q�[���p�b�h�{�^��
	/// </summary>
	/// <param name="Button">�����Ă���{�^��</param>
	/// <returns>�����Ă��邩</returns>
	bool PushButton(const int Button);

	/// <summary>
	/// �Q�[���p�b�h�{�^���i�������s�j
	/// </summary>
	/// <param name="Button">�����Ă���{�^��</param>
	/// <returns>�����Ă��邩</returns>
	bool TriggerButton(const int Button);

	/// <summary>
	/// ���g���K�[����������
	/// </summary>
	/// <returns>���������ۂ�</returns>
	bool PullLeftTigger();

	/// <summary>
	/// �E�g���K�[����������
	/// </summary>
	/// <returns>���������ۂ�</returns>
	bool PullRightTigger();
};