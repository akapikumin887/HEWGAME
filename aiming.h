#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "sprite.h"
#include "texture.h"

#ifndef AIMING_H_
#define AIMING_H_

#define AIMING_X 0.0f
#define AIMING_Y 0.0f
#define AIMING_Z 5.0f
#define AIMING_MAG_Z 6.0f
#define CHARGE_SPAN 50
#define TIME_COUNT_MAX 10.0f

class Aiming
{
public:
	VERTEX_3D *aimingv;         // ���_���
	Time *timec;                // ���ԏ��
	D3DXVECTOR3 pos;            // �ʒu
	D3DXVECTOR3 rot;            // ��]
	D3DXVECTOR3 scl;            // �傫��
	TextureIndex texture_index; // �e�N�X�`��
	bool revolution;            // ���]�t���O
	D3DXVECTOR3 revRadius;      // ���]���a
	D3DXVECTOR3 revSpd;         // ���]���x
	bool isAiming;              // �Ə��t���O
	bool prepare;               // �\���t���O
	bool isShot;                // ���˃t���O
	int charge_span;            // �`���[�W�Ԋu

	Aiming(); // Aiming�̒��_���擾�i�R���X�g���N�^�j
	~Aiming(); // Aiming�̒��_���̃����[�X�i�f�X�g���N�^�j

	void Draw_Aiming(); // Aiming�̕`��
	void Set_Aiming(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Aiming�̃e�N�X�`���E�ʒu�E�����i��]�j�E�傫���̐ݒ�
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Aiming�̌��]�t���O�E���]���a�E���]���x�̐ݒ�
	float Rotation_Correction(float r); // Aiming�̉�]�̕␳
	void TimeCount_On(); // ���Ԃ��J�E���g
};

void Aiming_Initialize(); // Aiming�̏�����
void Aiming_Finalize(); // Aiming�̏I������
void Aiming_Update(); // Aiming�̍X�V
void Aiming_Draw(); // Aiming�̕`��
Aiming* Get_Aiming(); // Aiming���̎擾
#endif