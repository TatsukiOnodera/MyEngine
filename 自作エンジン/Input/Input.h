#pragma once
#include "InputList.h"
#include "WinApp.h"
#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <DirectXMath.h>

class Input : public InputList
{
public: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

private: // メンバ変数
	// Direct Inputのバージョン指定
	constexpr static int dInputVersion = 0x0800;
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> dInput;
	// キーボードデバイス
	ComPtr<IDirectInputDevice8> devKeyboard;
	// キー判定
	BYTE keys[256] = {};
	// 前フレームのキー情報
	BYTE oldKeys[256] = {};
	// マウスデバイス
	ComPtr<IDirectInputDevice8> devMouse;
	// マウス判定
	DIMOUSESTATE2 mouseState = {};
	// 前フレームのマウス判定
	DIMOUSESTATE2 oldMouseState = {};
	// ゲームパッドデバイス
	ComPtr<IDirectInputDevice8> devGamePad;
	// ゲームパッドの判定
	DIJOYSTATE gamePadState = {};
	// 前フレームのゲームパッドの判定
	DIJOYSTATE oldGamePadState = {};
	// ボタンデータ
	bool isPush[32] = {};
	// スティックの反応範囲
	LONG responsiveRange = 100;
	// スティックの無反応範囲
	LONG unresponsiveRange = 40;

public: // 静的メンバ関数
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input* GetInstance();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キー操作
	/// </summary>
	//入力
	bool PushKey(BYTE key);
	//入力（長押し不可）
	bool TriggerKey(BYTE key);

	/// <summary>
	/// クリック
	/// </summary>
	//左クリック
	bool PushMouse(int Mouse);
	//左クリック（長押し不可）
	bool TriggerMouse(int Mouse);

	/// <summary>
	/// ゲームパッド
	/// </summary>
	//ゲームパッドスティック
	bool TiltLeftStick(int stick);
	//ゲームパッドスティック（長押し不可）
	bool TriggerLeftStick(int stick);
	//ゲームパッドスティックを倒した比率
	XMFLOAT2 LeftStickAngle();
	//ゲームパッドボタン
	bool PushButton(int Button);
	//ゲームパッドボタン（長押し不可）
	bool TriggerButton(int Button);
	//ゲームパッド十字キー
	bool PushCrossKey(int CrossKey);
	//ゲームパッド十字キー（長押し不可）
	bool TriggerCrossKey(int CrossKey);
};