#include "Input.h"
#include <assert.h>

using namespace DirectX;

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib, "xinput.lib")

Input* Input::GetInstance()
{
	static Input input;

	return &input;
}

void Input::Initialize(WinApp* win)
{
	HRESULT result;

	//�C���^�[�t�F�[�X�쐬
	result = DirectInput8Create(
		win->GetWindowInstance(), static_cast<DWORD>(s_dInputVersion), IID_IDirectInput8, (void**)&dInput, nullptr);
	if (FAILED(result))
	{
		assert(0);
	}

	//�L�[�{�[�h�f�o�C�X�̐���
	devKeyboard = nullptr;
	result = dInput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);

	//�}�E�X�̐���
	devMouse = nullptr;
	result = dInput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	if (devKeyboard != nullptr)
	{
		//���̓f�[�^�`���̃Z�b�g
		result = devKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
		{
			assert(0);
		}

		//�r�����䃌�x���Z�b�g
		result = devKeyboard->SetCooperativeLevel(win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
		if (FAILED(result))
		{
			assert(0);
		}
	}

	if (devMouse != nullptr)
	{
		//���̓f�[�^�`���̃Z�b�g
		result = devMouse->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(result))
		{
			assert(0);
		}

		//�r�����䃌�x���Z�b�g
		result = devMouse->SetCooperativeLevel(win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
		if (FAILED(result))
		{
			assert(0);
		}
	}

	Update();
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	if (devKeyboard != nullptr)
	{
		//�L�[�{�[�h���̎擾�J�n
		result = devKeyboard->Acquire();
		//�O�t���[���̃L�[���擾
		memcpy(oldKeys, keys, sizeof(keys));
		//�S�L�[�̏����擾����
		result = devKeyboard->GetDeviceState(sizeof(keys), keys);
	}

	if (devMouse != nullptr)
	{
		//�}�E�X���̎擾�J�n
		result = devMouse->Acquire();
		//�O�t���[���̔���
		oldMouseState = mouseState;
		//�S�N���b�N�̏����擾����
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	oldGamePadState = gamePadState;
	DWORD dwResult = XInputGetState(0, &gamePadState);
}

bool Input::PushKey(BYTE key)
{
	assert(0 <= key && key < 256);

	if (keys[key])
	{
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE key)
{
	assert(0 <= key && key < 256);

	if (keys[key] && !oldKeys[key])
	{
		return true;
	}
	return false;
}

bool Input::PushMouse(int Mouse)
{
	assert(0 <= Mouse && Mouse < 3);

	if (mouseState.rgbButtons[Mouse])
	{
		return true;
	}

	return false;
}

bool Input::TriggerMouse(int Mouse)
{
	assert(0 <= Mouse && Mouse < 3);

	if (mouseState.rgbButtons[Mouse] && !oldMouseState.rgbButtons[Mouse])
	{
		return true;
	}

	return false;
}

XMFLOAT2 Input::LeftStickAngle()
{
	// �f�b�g�]�[���̔���
	if ((gamePadState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && gamePadState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		 (gamePadState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && gamePadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		return XMFLOAT2(0, 0);
	}
	else
	{
		return XMFLOAT2(static_cast<float>(gamePadState.Gamepad.sThumbLX) / 32768, static_cast<float>(gamePadState.Gamepad.sThumbLY) / 32768);
	}
}

XMFLOAT2 Input::RightStickAngle()
{
	// �f�b�g�]�[���̔���
	if ((gamePadState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && gamePadState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(gamePadState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && gamePadState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return XMFLOAT2(0, 0);
	}
	else
	{
		return XMFLOAT2(static_cast<float>(gamePadState.Gamepad.sThumbRX) / 32768, static_cast<float>(gamePadState.Gamepad.sThumbRY) / 32768);
	}
}

bool Input::PushButton(const int Button)
{
	// �����Ă��邩
	if (gamePadState.Gamepad.wButtons & Button)
	{
		return true;
	}

	return false;
}

bool Input::TriggerButton(int Button)
{
	// �����Ă��邩
	if (gamePadState.Gamepad.wButtons & Button && !(oldGamePadState.Gamepad.wButtons & Button))
	{
		return true;
	}

	return false;
}

bool Input::PullLeftTigger()
{
	if (gamePadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::PullRightTigger()
{
	if (gamePadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return true;
	}
	else
	{
		return false;
	}
}
