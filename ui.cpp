#include "ui.h"
#include "score.h"

// UI�̏�����
void UI_Initialize()
{
	Score_Initialize();
}

// UI�̏I������
void UI_Finalize()
{
	Score_Finalize();
}

// UI�̍X�V
void UI_Update()
{
	Score_Update();
}

// UI�̕`��
void UI_Draw()
{
	Score_Draw();
}

Number::Number()
{
	n = 0;
	n_tmp = 0;
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
	TextureIndex = TEXTURE_INDEX_NUMBER;
	pos = D3DXVECTOR3(Texture_GetWidth(TextureIndex) / 10 / 2.0f, Texture_GetHeight(TextureIndex) / 2.0f, 0.0f);
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex) / 10;
	th = Texture_GetHeight(TextureIndex);
}

Alphabet::Alphabet()
{
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
	TextureIndex = TEXTURE_INDEX_ALPHABET;
	pos = D3DXVECTOR3(Texture_GetWidth(TextureIndex) / 13 / 2.0f, Texture_GetHeight(TextureIndex) / 2 / 2.0f, 0.0f);
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex) / 13;
	th = Texture_GetHeight(TextureIndex) / 2;
}