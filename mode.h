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

#ifndef MODE_H_
#define MODE_H_

typedef enum {
	MODE_NONE,
	MODE_EASY,
	MODE_HARD
}ModeIndex;

class Mode
{
public:
	bool bUse;                 // �g�p���t���O
	D3DXVECTOR3 pos;           // �\�����W
	D3DCOLOR color;	           // �J���[
	TextureIndex TextureIndex; // �e�N�X�`��ID
	int	tx, ty;	               // �e�N�X�`���\��t��������W
	int	tw, th;	               // �e�N�X�`���\��t���T�C�Y

	Mode();
	~Mode();
};

void Mode_Initialize(); // Mode�̏�����
void Mode_Finalize(); // Mode�̏I������
void Mode_Update(); // Mode�̍X�V
void Mode_Draw(); // Mode�̕`��
ModeIndex GetMode(); // Mode���̎擾
#endif