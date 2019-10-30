#include "mode.h"

static Mode mode;

// Modeの初期化
void Mode_Initialize()
{

}

// Modeの終了処理
void Mode_Finalize()
{

}

// Modeの更新
void Mode_Update()
{

}

// Modeの描画
void Mode_Draw()
{

}

Mode::Mode()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = SCREEN_HEIGHT * 0.5f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_MODE;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

Mode::~Mode()
{

}