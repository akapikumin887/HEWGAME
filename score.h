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

#ifndef Score_H_
#define Score_H_

class Score
{
public:
	int score;
	int rank;
	D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);	   // �J���[
	TextureIndex TextureIndex = TEXTURE_INDEX_SCORE_NUMBER; // �e�N�X�`��ID
	D3DXVECTOR3 pos;
	int	tx=2, ty=2;	               // �e�N�X�`���\��t��������W
	int	tw= Texture_GetWidth(TextureIndex), th= Texture_GetHeight(TextureIndex);// �e�N�X�`���\��t���T�C�Y

};



void Score_Initialize(); // Score�̏�����
void Score_Finalize(); // Score�̏I������
void Score_Update(); // Score�̍X�V
void Score_Draw(); // Score�̕`��
void Add_Score(int s); // Score�̉��Z
int* GetScore(); // Score�̎擾
#endif