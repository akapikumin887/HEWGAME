#include "title.h"

static Title title;

// Titleの初期化
void Title_Initialize()
{

}

// Titleの終了処理
void Title_Finalize()
{

}

// Titleの更新
void Title_Update()
{

}

// Titleの描画
void Title_Draw()
{

}

Title::Title()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = SCREEN_HEIGHT * 0.5f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_TITLE;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

Title::~Title()
{

}