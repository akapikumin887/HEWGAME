#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#ifndef UI_H_
#define UI_H_

#define SCORE_DIGIT_MAX 3
#define TIME_DIGIT_MAX 5
#define DIGIT_MAX 5

class UI
{
public:
	TextureIndex texture_index; // �e�N�X�`��ID
	D3DXVECTOR2  size;          // �|���S���̃T�C�Y
	D3DXVECTOR2  pos;           // ���݈ʒu
	int          tx, ty;
	int          tw, th;
	D3DCOLOR     color;	        // �J���[

	// UI�̏�����
	virtual void Initialize(TextureIndex tex_idx,        // TextureIndex�̐ݒ�
		D3DXVECTOR2 s, D3DXVECTOR2 p,                    // �T�C�Y�E�ʒu�̐ݒ�
		int t_x = 0, int t_y = 0,                        // t_x, t_y�؂���J�n�ʒu(�f�t�H���g�l��0: �e�N�X�`�����ォ��؂���)
		int t_w = 1, int t_h = 1,                        // t_w, t_h�؂���P��(�f�t�H���g�l��1: �e�N�X�`���̉E���܂Ő؂���)
		D3DCOLOR c = D3DCOLOR_RGBA(255, 255, 255, 255)); // �F�̐ݒ�
	virtual void Finalize();   // UI�̏I������
	virtual void Update();     // UI�̍X�V
	virtual void Draw();       // UI�̕`��
};

class Number :public UI
{
public:
	int n;

	Number();
};

class Alphabet :public UI
{
public:
	int  len;
	char word[8];

	Alphabet();
};
#endif