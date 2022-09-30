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
public: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;

private: // メンバ変数
	// Direct Inputのバージョン指定
	constexpr static int s_dInputVersion = 0x0800;
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
	// ゲームパッドの判定
	XINPUT_STATE gamePadState = {};
	// 前フレームのゲームパッドの判定
	XINPUT_STATE oldGamePadState = {};

public: // 静的メンバ関数
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input* GetInstance();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キー入力
	/// </summary>
	/// <param name="key">押しているキー</param>
	/// <returns>押しているか</returns>
	bool PushKey(BYTE key);

	/// <summary>
	/// キー入力（長押し不可）
	/// </summary>
	/// <param name="key">押しているキー</param>
	/// <returns>押しているか</returns>
	bool TriggerKey(BYTE key);

	/// <summary>
	/// 左クリック
	/// </summary>
	/// <param name="Mouse">押しているボタン</param>
	/// <returns>押しているか</returns>
	bool PushMouse(const int Mouse);

	/// <summary>
	/// 右クリック
	/// </summary>
	/// <param name="Mouse">押しているボタン</param>
	/// <returns>押しているか</returns>
	bool TriggerMouse(const int Mouse);
	
	/// <summary>
	/// ゲームパッド左スティックを倒した割合
	/// </summary>
	/// <returns>スティックを倒した割合</returns>
	XMFLOAT2 LeftStickAngle();

	/// <summary>
	/// ゲームパッド左スティックを倒した割合
	/// </summary>
	/// <returns>スティックを倒した割合</returns>
	XMFLOAT2 RightStickAngle();

	/// <summary>
	/// ゲームパッドボタン
	/// </summary>
	/// <param name="Button">押しているボタン</param>
	/// <returns>押しているか</returns>
	bool PushButton(const int Button);

	/// <summary>
	/// ゲームパッドボタン（長押し不可）
	/// </summary>
	/// <param name="Button">押しているボタン</param>
	/// <returns>押しているか</returns>
	bool TriggerButton(const int Button);

	/// <summary>
	/// 左トリガーを引いたか
	/// </summary>
	/// <returns>引いたか否か</returns>
	bool PullLeftTigger();

	/// <summary>
	/// 右トリガーを引いたか
	/// </summary>
	/// <returns>引いたか否か</returns>
	bool PullRightTigger();
};