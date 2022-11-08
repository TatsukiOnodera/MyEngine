#include "DebugText.h"

void DebugText::Initialize(const UINT texnumber)
{
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = Sprite::Create(texnumber);
	}
}

void DebugText::Print(const std::string& text, const float x, const float y, const float scale)
{
	for (int i = 0; i < text.size(); i++)
	{
		if (spriteIndex >= c_maxCharCount)
		{
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / c_fontLineCount;
		int fontIndexX = fontIndex % c_fontLineCount;

		sprites[spriteIndex]->SetPosition({ x + c_fontWidth * scale * i, y});
		sprites[spriteIndex]->SetLeftTop({ (float)fontIndexX * c_fontWidth, (float)fontIndexY * c_fontHeight });
		sprites[spriteIndex]->SetTexSize({ c_fontWidth, c_fontHeight });
		sprites[spriteIndex]->SetSize({ c_fontWidth * scale, c_fontHeight * scale });

		sprites[spriteIndex]->SpriteTransferVertexBuffer();

		sprites[spriteIndex]->Update();

		spriteIndex++;
	}
}

void DebugText::Draw(ID3D12GraphicsCommandList* cmdList)
{
	Sprite::PreDraw(cmdList);

	for (int i = 0; i < spriteIndex; i++)
	{
		sprites[i]->Draw();
	}

	spriteIndex = 0;

	Sprite::PostDraw();
}