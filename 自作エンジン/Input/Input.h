#pragma once
#include <windows.h>
#include <dinput.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION             0x0800 //Direct Input�̃o�[�W�����w��

class Input
{
public: //�G�C���A�X
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�萔
	//�Q�[���p�b�h�i�{�^���j
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LS,
		Button_RS,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};
	//�}�E�X
	enum MouseKind
	{
		M_Left, M_Right, M_Middle
	};
	//�X�e�B�b�N
	enum StickKind
	{
		S_Up, S_Down, S_Right, S_Left
	};

private: //�����o�ϐ�
	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�L�[����
	BYTE keys[256] = {};
	//�O�t���[���̃L�[���
	BYTE oldkeys[256] = {};
	//�}�E�X�f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;
	//�}�E�X����
	DIMOUSESTATE2 mouseState = {};
	//�O�t���[���̃}�E�X����
	DIMOUSESTATE2 oldMouseState = {};
	//�Q�[���p�b�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devGamePad;
	//�Q�[���p�b�h�̔���
	DIJOYSTATE gamePadState = {};
	//�O�t���[���̃Q�[���p�b�h�̔���
	DIJOYSTATE oldGamePadState = {};
	//�{�^���f�[�^
	bool is_push[32] = {};
	//�X�e�B�b�N�̖������͈�
	LONG unresponsive_range = 200;

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hwnd"></param>
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/*�L�[����*/
	//����
	bool PushKey(BYTE key);
	//���́i�������s�j
	bool TriggerKey(BYTE key);

	/*�N���b�N*/
	//���N���b�N
	bool PushMouse(int Mouse);
	//���N���b�N�i�������s�j
	bool TriggerMouse(int Mouse);

	/*�Q�[���p�b�h*/
	//�Q�[���p�b�h�X�e�B�b�N
	bool TiltStick(int stick);
	//�Q�[���p�b�h�X�e�B�b�N�i�������s�j
	bool TriggerStick(int stick);
	//�Q�[���p�b�h�{�^��
	bool PushButton(int Button);
	//�Q�[���p�b�h�{�^���i�������s�j
	bool TriggerButton(int Button);
	//�Q�[���p�b�h�\���L�[
	bool PushCrossKey(int CrossKey);
	//�Q�[���p�b�h�\���L�[�i�������s�j
	bool TriggerCrossKey(int CrossKey);
};