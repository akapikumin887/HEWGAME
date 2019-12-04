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

#ifndef CUBE_H_
#define CUBE_H_

#define CUBE_MAX 5
#define MOVE_SPEED 10.0f
#define CUBE_X AIMING_X
#define CUBE_Y 3.5f
#define CUBE_Z AIMING_Z
#define CUBE_MAG_Z 3.5f

class Cube
{
public:
	bool bUse;                  // �g�p�t���O
	bool bShotted;              // ���˃t���O
	bool nShotted;              // ���Ԑ����Ŗ����˃t���O
	bool bHit;                  // �����t���O
	VERTEX_3D *cubev;           // ���_���
	TextureIndex texture_index; // �e�N�X�`��
	D3DXVECTOR3 pos;            // ���݈ʒu
	D3DXVECTOR3 posOld;         // ���ʒu
	D3DXVECTOR3 posHead;        // �擪�ʒu
	D3DXVECTOR3 posHit;         // �I�ł̎h���ʒu
	D3DXVECTOR3 posAiming;      // ���ˎ���Aiming�̈ʒu���
	D3DXVECTOR3 rot;            // ��]
	D3DXVECTOR3 scl;            // �傫��
	bool revolution;            // ���]�t���O
	D3DXVECTOR3 revRadius;      // ���]���a
	D3DXVECTOR3 revSpd;         // ���]���x
	D3DXVECTOR3 direction;      // Cube�̌���
	D3DXVECTOR3 move;           // Cube�̈ړ���
	static int cnt;             // ��̎c��
	float targetLen;            // ��̐�[�ʒu�ƓI�̒��S�ʒu�Ƃ̋���

	Cube(); // Cube�̒��_���擾�i�R���X�g���N�^�j
	~Cube(); // Cube�̒��_���̃����[�X�i�f�X�g���N�^�j

	void Draw_Cube(); // Cube�̕`��
	void Set_Cube(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Cube�̃e�N�X�`���E�ʒu�E��]�E�傫���̐ݒ�
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Cube�̌��]�t���O�E���]���a�E���]���x�̐ݒ�
	void Cube_Aiming_Direction(); // Cube�̔��ˎ��̌���
	//void Cube_Move_Direction(D3DXVECTOR3 wind,float g = GRAVILITY / GRAVILITY_FORCE_MAG); // Cube�̔��ˎ��̌���
	void Cube_Move_Direction(); // Cube�̔��ˎ��̌���
	float Rotation_Correction(float r); // Cube�̉�]�̕␳
	float Get_Length(D3DXVECTOR3 l); // �����̌v�Z
	void Score(); // Score����
};

void Cube_Initialize(); // Cube�̏�����
void Cube_Finalize(); // Cube�̏I������
void Cube_Update(); // Cube�̍X�V
void Cube_Draw(); // Cube�̕`��
void Add_Cube(); // Cube�̒ǉ�
Cube* Get_Cube(); // Cube���̎擾
#endif