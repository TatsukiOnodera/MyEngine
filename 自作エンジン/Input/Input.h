#pragma once
#include <windows.h>
#include <dinput.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION             0x0800 //Direct Inputのバージョン指定

class Input
{
public: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //定数
	//ゲームパッド（ボタン）
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
	//マウス
	enum MouseKind
	{
		M_Left, M_Right, M_Middle
	};
	//スティック
	enum StickKind
	{
		S_Up, S_Down, S_Right, S_Left
	};

private: //メンバ変数
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//キーボードデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//キー判定
	BYTE keys[256] = {};
	//前フレームのキー情報
	BYTE oldkeys[256] = {};
	//マウスデバイス
	ComPtr<IDirectInputDevice8> devMouse;
	//マウス判定
	DIMOUSESTATE2 mouseState = {};
	//前フレームのマウス判定
	DIMOUSESTATE2 oldMouseState = {};
	//ゲームパッドデバイス
	ComPtr<IDirectInputDevice8> devGamePad;
	//ゲームパッドの判定
	DIJOYSTATE gamePadState = {};
	//前フレームのゲームパッドの判定
	DIJOYSTATE oldGamePadState = {};
	//ボタンデータ
	bool is_push[32] = {};
	//スティックの無反応範囲
	LONG unresponsive_range = 200;

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hwnd"></param>
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/*キー操作*/
	//入力
	bool PushKey(BYTE key);
	//入力（長押し不可）
	bool TriggerKey(BYTE key);

	/*クリック*/
	//左クリック
	bool PushMouse(int Mouse);
	//左クリック（長押し不可）
	bool TriggerMouse(int Mouse);

	/*ゲームパッド*/
	//ゲームパッドスティック
	bool TiltStick(int stick);
	//ゲームパッドスティック（長押し不可）
	bool TriggerStick(int stick);
	//ゲームパッドボタン
	bool PushButton(int Button);
	//ゲームパッドボタン（長押し不可）
	bool TriggerButton(int Button);
	//ゲームパッド十字キー
	bool PushCrossKey(int CrossKey);
	//ゲームパッド十字キー（長押し不可）
	bool TriggerCrossKey(int CrossKey);
};