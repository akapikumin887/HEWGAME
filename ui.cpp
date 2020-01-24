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
void UI::Draw(float r)
{
	Sprite_SetColor_2D(color); // 色のセット
	Sprite_Draw_Matrix_2D(texture_index, size, pos, tx, ty, tw, th, r);
}

// Number
// Numberの初期化
void Number::Initialize(TextureIndex tex_idx, D3DXVECTOR2 s, D3DXVECTOR2 p, int t_x, int t_y, int t_w, int t_h, int di, int dd, D3DCOLOR c)
{
	texture_index = tex_idx;
	size = s;
	pos = p;
	tx = t_x;
	ty = t_y;
	tw = Texture_GetWidth(texture_index) / t_w;
	th = Texture_GetHeight(texture_index) / t_h;
	digit_i = di;
	digit_d = dd;
	color = c;
}

// Numberの描画
void Number::Draw(double n, int di, int dd, float r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Sprite_SetColor_2D(color); // 色のセット

	// Numberの表示
	// 整数部
	for (int i = 0; i < di; i++)
	{
		int tmp;

		// 最上位の位から始まる
		tmp = (int)(n / powf((float)10, (float)(di - 1 - i)));
		tmp = tmp % 10; // 1の位の値を取り出す

		Sprite_Draw_Matrix_2D(texture_index, size,
			D3DXVECTOR2(pos.x - tw * di + tw * i, pos.y),
			tx + tmp * tw, ty, tw, th, r);
	}
	// 小数部
	for (int i = 0; i < dd; i++)
	{
		int tmp;

		// 小数点以降1桁目から始まる
		tmp = (int)(n / pow((double)10, -(double)(i + 1)));
		tmp = tmp % 10; // 1の位の値を取り出す

		Sprite_Draw_Matrix_2D(texture_index, size,
			D3DXVECTOR2(pos.x + tw * (di - 1) + tw * i, pos.y),
			tx + tmp * tw, ty, tw, th, r);
	}
}

// Alphabet
// Alphabetの初期化
void Alphabet::Initialize(TextureIndex tex_idx, string w, D3DXVECTOR2 s, D3DXVECTOR2 p, int t_x, int t_y, int t_w, int t_h, int blank, D3DCOLOR c)
{
	texture_index = tex_idx;
	word = w;
	pos = p;
	size = s;
	tx = t_x;
	ty = t_y;
	tw = Texture_GetWidth(texture_index) / t_w;
	th = Texture_GetHeight(texture_index) / t_h;
	color = c;
	pos.x = p.x - (word.size() + blank) * th;
}

// Alphabetの描画
void Alphabet::Draw(float r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Sprite_SetColor_2D(color); // 色のセット

	// アルファベットの表示
	for (int i = 0; i < word.size(); i++)
	{
		Sprite_Draw_Matrix_2D(texture_index, size,
			D3DXVECTOR2(pos.x + tw * i, pos.y),
			tx + ((word[i] - 65) % 13) * tw, ty + (word[i] - 65) / 13 * th, tw, th, r);
	}
}