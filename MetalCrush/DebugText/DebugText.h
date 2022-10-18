#pragma once
#include "Sprite.h"
#include <string>

class DebugText
{
public:
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	Sprite* sprites[maxCharCount] = {};
	int spriteIndex = 0;
};