#include "ui.h"
#include "score.h"

// UI
// UI�̏�����
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

// UI�̏I������
void UI::Finalize()
{

}

// UI�̍X�V
void UI::Update()
{

}

// UI�̕`��
void UI::Draw(float r)
{
	Sprite_SetColor_2D(color); // �F�̃Z�b�g
	Sprite_Draw_Matrix_2D(texture_index, size, pos, tx, ty, tw, th, r);
}

// Number
// Number�̏�����
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

// Number�̕`��
void Number::Draw(double n, int di, int dd, float r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Sprite_SetColor_2D(color); // �F�̃Z�b�g

	// Number�̕\��
	// ������
	for (int i = 0; i < di; i++)
	{
		int tmp;

		// �ŏ�ʂ̈ʂ���n�܂�
		tmp = (int)(n / powf((float)10, (float)(di - 1 - i)));
		tmp = tmp % 10; // 1�̈ʂ̒l�����o��

		Sprite_Draw_Matrix_2D(texture_index, size,
			D3DXVECTOR2(pos.x - tw * di + tw * i, pos.y),
			tx + tmp * tw, ty, tw, th, r);
	}
	// ������
	for (int i = 0; i < dd; i++)
	{
		int tmp;

		// �����_�ȍ~1���ڂ���n�܂�
		tmp = (int)(n / pow((double)10, -(double)(i + 1)));
		tmp = tmp % 10; // 1�̈ʂ̒l�����o��

		Sprite_Draw_Matrix_2D(texture_index, size,
			D3DXVECTOR2(pos.x + tw * (di - 1) + tw * i, pos.y),
			tx + tmp * tw, ty, tw, th, r);
	}
}

// Alphabet
// Alphabet�̏�����
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

// Alphabet�̕`��
void Alphabet::Draw(float r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Sprite_SetColor_2D(color); // �F�̃Z�b�g

	// �A���t�@�x�b�g�̕\��
	for (int i = 0; i < word.size(); i++)
	{
		Sprite_Draw_Matrix_2D(texture_index, size,
			D3DXVECTOR2(pos.x + tw * i, pos.y),
			tx + ((word[i] - 65) % 13) * tw, ty + (word[i] - 65) / 13 * th, tw, th, r);
	}
}