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

#ifndef TITLE_H_
#define TITLE_H_

class Title
{
public:
	bool bUse;                 // �g�p���t���O
	D3DXVECTOR3 pos;           // �\�����W
	D3DCOLOR color;	           // �J���[
	TextureIndex Texture_Index; // �e�N�X�`��ID
	int	tx, ty;	               // �e�N�X�`���\��t��������W
	int	tw, th;	               // �e�N�X�`���\��t���T�C�Y

	Title();
	~Title();

	void Initialize(TextureIndex idx, D3DXVECTOR2 p = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	void Fade_Move_Up();
	void Fade_Move_Down();
};

void Title_Initialize(); // Title�̏�����
void Title_Finalize(); // Title�̏I������
void Title_Update(); // Title�̍X�V
void Title_Draw(); // Title�̕`��

#endif