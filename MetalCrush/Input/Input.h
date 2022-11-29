#pragma once
#include "WinApp.h"

#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>
#include <Xinput.h>

/// <summary>
/// �Q�[���p�b�h�z�u�iXBOX�j
/// </summary>
enum BUTTON
{
	// �\��
	U_CROSS = 0x0001,
	D_CROSS = 0x0002,
	L_CROSS = 0x0004,
	R_CROSS = 0x0008,
	// �����E
	START = 0x0010,
	// ������
	BACK = 0x0020,
	// �X�e�B�b�N
	L_STICK = 0x0040,
	R_STICK = 0x0080,
	// �{�^��
	LB = 0x0100,
	RB = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
};

/// <summary>
/// �}�E�X�z�u
/// </summary>
enum MOUSE
{
	LEFT, RIGHT, MIDDLE
};

/// <summary>
/// ���͏���
/// </summary>
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
	bool PushKey(const BYTE key);

	/// <summary>
	/// �L�[���́i�������s�j
	/// </summary>
	/// <param name="key">�����Ă���L�[</param>
	/// <returns>�����Ă��邩</returns>
	bool TriggerKey(const BYTE key);

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
	/// ���g���K�[�������Ă��邩
	/// </summary>
	/// <returns>�����Ă��邩�ۂ�</returns>
	bool PullLeftTrigger();

	/// <summary>
	/// �E�g���K�[�������Ă��邩
	/// </summary>
	/// <returns>�����Ă��邩�ۂ�</returns>
	bool PullRightTrigger();

	/// <summary>
	/// ���g���K�[�������Ă��邩�i�������s�j
	/// </summary>
	/// <returns>�����Ă��邩�ۂ�</returns>
	bool SwitchLeftTrigger();

	/// <summary>
	/// �E�g���K�[�������Ă��邩�i�������s�j
	/// </summary>
	/// <returns>�����Ă��邩�ۂ�</returns>
	bool SwitchRightTrigger();
};