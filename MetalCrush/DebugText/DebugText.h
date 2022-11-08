#pragma once
#include "Sprite.h"

#include <string>

class DebugText
{
public: // 定数
	// 最大表示数
	static const int c_maxCharCount = 256;
	// 横のサイズ
	static const int c_fontWidth = 9;
	// 縦のサイズ
	static const int c_fontHeight = 18;
	// 一行の長さ
	static const int c_fontLineCount = 14;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texnumber">テクスチャの番号</param>
	void Initialize(const UINT texnumber);

	/// <summary>
	/// 画面に表示する
	/// </summary>
	/// <param name="text">表示文字</param>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="scale">スケール</param>
	void Print(const std::string& text, const float x, const float y, const float scale = 1.0f);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	// 文字を描画するスプライト
	Sprite* sprites[c_maxCharCount] = {};
	// 指定の文字
	int spriteIndex = 0;
};