#pragma once
#include "InputList.h"
#include "WinApp.h"
#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <DirectXMath.h>

class Input : public InputList
{
public: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

private: // �����o�ϐ�
	// Direct Input�̃o�[�W�����w��
	constexpr static int dInputVersion = 0x0800;
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
	// �Q�[���p�b�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devGamePad;
	// �Q�[���p�b�h�̔���
	DIJOYSTATE gamePadState = {};
	// �O�t���[���̃Q�[���p�b�h�̔���
	DIJOYSTATE oldGamePadState = {};
	// �{�^���f�[�^
	bool isPush[32] = {};
	// �X�e�B�b�N�̔����͈�
	LONG responsiveRange = 100;
	// �X�e�B�b�N�̖������͈�
	LONG unresponsiveRange = 40;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Input* GetInstance();

public: //�����o�֐�
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
	//����
	bool PushKey(BYTE key);
	//���́i�������s�j
	bool TriggerKey(BYTE key);

	/// <summary>
	/// �N���b�N
	/// </summary>
	//���N���b�N
	bool PushMouse(int Mouse);
	//���N���b�N�i�������s�j
	bool TriggerMouse(int Mouse);

	/// <summary>
	/// �Q�[���p�b�h
	/// </summary>
	//�Q�[���p�b�h�X�e�B�b�N
	bool TiltLeftStick(int stick);
	//�Q�[���p�b�h�X�e�B�b�N�i�������s�j
	bool TriggerLeftStick(int stick);
	//�Q�[���p�b�h�X�e�B�b�N��|�����䗦
	XMFLOAT2 LeftStickAngle();
	//�Q�[���p�b�h�{�^��
	bool PushButton(int Button);
	//�Q�[���p�b�h�{�^���i�������s�j
	bool TriggerButton(int Button);
	//�Q�[���p�b�h�\���L�[
	bool PushCrossKey(int CrossKey);
	//�Q�[���p�b�h�\���L�[�i�������s�j
	bool TriggerCrossKey(int CrossKey);
};