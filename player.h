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

#ifndef PLAYER_H_
#define PLAYER_H_

class Player
{
public:
	bool bUse;                 // �g�p���t���O
	D3DXVECTOR3 pos;           // �\�����W
	D3DCOLOR color;	           // �J���[
	TextureIndex TextureIndex; // �e�N�X�`��ID
	int	tx, ty;	               // �e�N�X�`���\��t��������W
	int	tw, th;	               // �e�N�X�`���\��t���T�C�Y
	int power;                 // �|��������
	int	charge_span;           // �`���[�W�Ԋu

	Player();
	~Player();
};

void Player_Initialize(); // Player�̏�����
void Player_Finalize(); // Player�̏I������
void Player_Update(); // Player�̍X�V
void Player_Draw(); // Player�̕`��
#endif