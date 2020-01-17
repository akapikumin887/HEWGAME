#include "ui.h"
#include "score.h"

// UI
// UIの初期化
void UI::Initialize(TextureIndex tex_idx, D3DXVECTOR2 s, D3DXVECTOR2 p,int t_x,int t_y,int t_w,int t_h, D3DCOLOR c)
{
	texture_index = tex_idx;
	size = s;
	pos = p;
	tx = t_x;
	ty = t_y;
	tw = Texture_GetWidth(texture_index) / t_w;
	th = Texture_GetHeight(texture_index) / t_h;
	color = c;
}

// UIの終了処理
void UI::Finalize()
{

}

// UIの更新
void UI::Update()
{

}

// UIの描画
void UI::Draw()
{
	Sprite_SetColor_2D(color); // 色のセット
	Sprite_Draw_Matrix_2D(texture_index, size, pos, tx, ty, tw, th, 0.0f);
}






Number::Number()
{
	n = 0;
	texture_index = TEXTURE_INDEX_NUMBER;
	pos = D3DXVECTOR2(Texture_GetWidth(texture_index) / 10 / 2.0f, Texture_GetHeight(texture_index) / 2.0f);
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(texture_index) / 10;
	th = Texture_GetHeight(texture_index);
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
}

Alphabet::Alphabet()
{
	texture_index = TEXTURE_INDEX_ALPHABET;
	pos = D3DXVECTOR2(Texture_GetWidth(texture_index) / 13 / 2.0f, Texture_GetHeight(texture_index) / 2 / 2.0f);
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(texture_index) / 13;
	th = Texture_GetHeight(texture_index) / 2;
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
}