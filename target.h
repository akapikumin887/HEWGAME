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

#ifndef TARGET_H_
#define TARGET_H_

class Target
{
public:
	bool bUse;                 // �g�p���t���O
	D3DXVECTOR3 pos;           // �\�����W
	D3DCOLOR color;	           // �J���[
	TextureIndex TextureIndex; // �e�N�X�`��ID
	int	tx, ty;	               // �e�N�X�`���\��t��������W
	int	tw, th;	               // �e�N�X�`���\��t���T�C�Y

	Target();
	~Target();
};

void Target_Initialize(); // Target�̏�����
void Target_Finalize(); // Target�̏I������
void Target_Update(); // Target�̍X�V
void Target_Draw(); // Target�̕`��
Target* GetTarget(); // Target�̎擾
#endif