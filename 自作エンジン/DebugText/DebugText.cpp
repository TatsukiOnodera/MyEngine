#include "DebugText.h"

void DebugText::Initialize(UINT texnumber)
{
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = Sprite::CreateSprite(texnumber);
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	for (int i = 0; i < text.size(); i++)
	{
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		sprites[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y, 0});
		sprites[spriteIndex]->SetLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		sprites[spriteIndex]->SetTexSize({ fontWidth, fontHeight });
		sprites[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		sprites[spriteIndex]->SpriteTransferVertexBuffer();

		sprites[spriteIndex]->Update();

		spriteIndex++;
	}
}

void DebugText::Draw()
{
	for (int i = 0; i < spriteIndex; i++)
	{
		sprites[i]->Draw();
	}

	spriteIndex = 0;
}