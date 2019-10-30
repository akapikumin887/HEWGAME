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

#ifndef ARROW_H_
#define ARROW_H_

#define ARROW_MAX 5
#define ARROW_X SCREEN_WIDTH * 0.5f
#define ARROW_Y SCREEN_HEIGHT * 0.9f
#define ARROW_SPEED 0.5f

class Arrow
{
public:
	bool bUse;                 // �g�p���t���O
	bool beShotted;            // ���˃t���O
	bool hit;                  // �����t���O
	D3DXVECTOR3 posTail;       // ��̔��̍��W
	D3DXVECTOR3 posHead;       // ��̐擪�̍��W
	D3DXVECTOR3 direction;     // ��̌���
	D3DXVECTOR3 move;          // ��̈ړ���
	D3DXVECTOR3 degree;        // ���ˊp�x
	D3DCOLOR color;	           // �J���[
	TextureIndex TextureIndex; // �e�N�X�`��ID
	int	tx, ty;	               // �e�N�X�`���\��t��������W
	int	tw, th;	               // �e�N�X�`���\��t���T�C�Y
	int charge;                // ��ɗ^����ꂽ��
	static int cnt;            // ��̐��̃J�E���^

	Arrow();
	~Arrow();
	void Arrow_Direction_Normalize(); // �����x�N�g���̎擾�Ɛ��K��
	void Arrow_Head_Pos(); // ��̐擪���W�̎擾
	static void Print(); // �c��̖{���̕\��
};

void Arrow_Initialize(); // ARROW�̏�����
void Arrow_Finalize(); // ARROW�̏I������
void Arrow_Update(); // ARROW�̍X�V
void Arrow_Draw(); // ARROW�̕`��
void Create_Arrow(); // ARROW�̐���
Arrow* GetArrow(); // ARROW�̏��擾
#endif