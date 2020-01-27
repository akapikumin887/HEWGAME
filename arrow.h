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
#include "xmodel.h"
#include "gravility.h"
#include "wind.h"
#include "ui.h"
#include "score.h"

#define ARROWXMODEL "asset/xmodel/bow.x"

#define ARROW_SIZE_X 0.025f
#define ARROW_SIZE_Y 0.025f
#define ARROW_SIZE_Z 5.0f

#define ARROW_POS_X 0.0f
#define ARROW_POS_Y 0.0f
#define ARROW_POS_Z -20.0f
#define ARROW_POS_Z_MAX 1000.0f

#define ARROW_ROT_X 0.0f
#define ARROW_ROT_Y 0.0f
#define ARROW_ROT_Z 0.0f

#define ARROW_MAX 5
#define ARROW_MOVE_SPEED 8.0f

enum ArrowState {
	ARROW_STATE_NONE,
	ARROW_STATE_WAIT_ZOOM,
	ARROW_STATE_PREPARE,
	ARROW_STATE_FLYING,
	ARROW_STATE_HIT_TARGET,
	ARROW_STATE_HIT_PLANE,
	ARROW_STATE_OVER_FLYING,
};

class BowXModel
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	XModel       *xmodel;       // XModel

	D3DXVECTOR3 posAiming; // ���ˎ���Aiming�̈ʒu���
	D3DXVECTOR3 direction; // ����
	D3DXVECTOR3 move;      // �ړ���

	static ArrowState state; // BowXModel�̏��

	BowXModel();  // BowXModel�̏������i�R���X�g���N�^�j
	~BowXModel(); // BowXModel�̏I�������i�f�X�g���N�^�j

	void Initialize(const char *filepass, TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r); // BowXModel�̏�����
	void Finalize();   // BowXModel�̏I������
	void Update();     // BowXModel�̍X�V
	void Draw();       // BowXModel�̕`��

	void Aiming_Direction(); // BowXModel�̌���
};

class Arrow
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	Cube         *cube;         // Cube

	D3DXVECTOR3  posOld;        // ���ʒu
	D3DXVECTOR3  posAiming;     // ���ˎ���Aiming�̈ʒu���
	D3DXVECTOR3  posHead;       // �擪�ʒu
	D3DXVECTOR3  posHit;        // �I�ł̎h���ʒu
	D3DXVECTOR3  direction;     // Arrow�̌���
	D3DXVECTOR3  move;          // Arrow�̈ړ���

	ArrowState   state;         // Arrow�̏��

	Gravility    *gravility;    // �d��
	Wind         *wind;         // ��

	bool         bUse;          // �g�p�t���O
	bool         Display;       // �\���t���O

	float        targetLen;     // ��̐�[�ʒu�ƓI�̒��S�ʒu�Ƃ̋���
	int          score;         // Score

	static float speed;         // ���x

	Arrow();  // Arrow�̏������i�R���X�g���N�^�j
	~Arrow(); // Arrow�̏I�������i�f�X�g���N�^�j

	void Initialize(); // Arrow�̏�����
	void Finalize(); // Arrow�̏I������
	void Update();   // Arrow�̍X�V
	void Draw();     // Arrow�̕`��

	void Aiming_Direction(); // Arrow�̔��ˎ��̌���
	void Move_Direction();   // Arrow�̈ړ����̌���
	void Movement();  // Arrow�̈ړ�
	void HitCheck();  // �����蔻��
	int  Get_Score(); // Score�̎擾
};

class ArrowManager
{
public:
	static Arrow *arrows; // Arrow�̔z��
	static int   num;     // Arrow�̐�
	static int   cnt;     // ��̎c��
	static int   score_t; // Score

	static void   Initialize(int n); // �S�Ă�Arrow�̏�����
	static void   Finalize();        // �S�Ă�Arrow�̏I������
	static void   Update();          // �S�Ă�Arrow�̍X�V
	static void   Draw();            // �S�Ă�Arrow�̕`��
	static void   Add_Arrow();       // Arrow�̐���
	static Arrow* Get_Arrow();       // Arrow���̎擾
};