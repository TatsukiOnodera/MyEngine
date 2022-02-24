#pragma once
#include <Windows.h>

class WinApp
{
public: //定数
	// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅
	static const wchar_t window_title[];  // ウィンドウタイトル

private: //メンバ変数
	HWND hwnd = nullptr; //ウィンドウオブジェクト
	WNDCLASSEX w{}; //ウィンドウクラスの設定

public: //メンバ関数
	/// <summary>
	/// ゲームウィンドウ生成
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// メッセージプロシージャ
	/// </summary>
	/// <returns>メッセージがあるか否か</returns>
	bool ProcessMessage();

	 /// <summary>
	 /// ウィンドウクラスを登録解除
	 /// </summary>
	 void TerminateGameWindow();

	/// <summary>
	/// ウィンドウプロシージャの定義
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns>標準の処理を行うか否か</returns>
	static LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// HWNDのアクセッサ
	/// </summary>
	/// <returns></returns>
	HWND GetHWND() { return hwnd; };

	/// <summary>
	/// WindowsInstanceのアクセッサ
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetWindowInstance() { return w.hInstance; };
};