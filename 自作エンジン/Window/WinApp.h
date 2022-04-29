#pragma once
#include <Windows.h>

class WinApp
{
public: //�萔
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��
	static const wchar_t window_title[];  // �E�B���h�E�^�C�g��

private: //�����o�ϐ�
	HWND hwnd = nullptr; //�E�B���h�E�I�u�W�F�N�g
	WNDCLASSEX w{}; //�E�B���h�E�N���X�̐ݒ�

public: //�ÓI�����o�֐�
	static WinApp* GetInstance();

public: //�����o�֐�
	/// <summary>
	/// �Q�[���E�B���h�E����
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// ���b�Z�[�W�v���V�[�W��
	/// </summary>
	/// <returns>���b�Z�[�W�����邩�ۂ�</returns>
	bool ProcessMessage();

	 /// <summary>
	 /// �E�B���h�E�N���X��o�^����
	 /// </summary>
	 void TerminateGameWindow();

	/// <summary>
	/// �E�B���h�E�v���V�[�W���̒�`
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns>�W���̏������s�����ۂ�</returns>
	static LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// HWND�̃A�N�Z�b�T
	/// </summary>
	/// <returns></returns>
	HWND GetHWND() { return hwnd; };

	/// <summary>
	/// WindowsInstance�̃A�N�Z�b�T
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetWindowInstance() { return w.hInstance; };
};