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
	D3DCOLOR color;	           // �J���[
	TextureIndex TextureIndex; // �e�N�X�`��ID
	D3DXVECTOR3 pos;
	int tx, ty;
	int tw, th;
};

class Number :public UI
{
public:
	int n;
	int n_tmp;

	Number();
};

class Alphabet :public UI
{
public:
	int len;

	Alphabet();
};

void UI_Initialize(); // UI�̏�����
void UI_Finalize(); // UI�̏I������
void UI_Update(); // UI�̍X�V
void UI_Draw(); // UI�̕`��
#endif