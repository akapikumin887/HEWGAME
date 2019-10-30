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

#ifndef Result_H_
#define Result_H_

class Result
{
public:
	bool bUse;                 // �g�p���t���O
	D3DXVECTOR3	pos;	       //�\�����W
	D3DCOLOR color;	           // �J���[
	TextureIndex TextureIndex; // �e�N�X�`��ID
	int	tx, ty;	               // �e�N�X�`���\��t��������W
	int	tw, th;	               // �e�N�X�`���\��t���T�C�Y

	Result();
	~Result();
};

void Result_Initialize(); // Result�̏�����
void Result_Finalize(); // Result�̏I������
void Result_Update(); // Result�̍X�V
void Result_Draw(); // Result�̕`��
#endif