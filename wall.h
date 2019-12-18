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

#ifndef WALL_H_
#define WALL_H_

#define WALLMOVE 1.0f

class Wall
{
public:
	VERTEX_3D *wallv;            // ���_���
	TextureIndex texture_index; // �e�N�X�`��
	D3DXVECTOR3 pos;            // �ʒu
	D3DXVECTOR3 rot;            // ��]
	D3DXVECTOR3 scl;            // �傫��
	bool revolution;            // ���]�t���O
	D3DXVECTOR3 revRadius;      // ���]���a
	D3DXVECTOR3 revSpd;         // ���]���x

	Wall(); // Wall�̒��_���擾�i�R���X�g���N�^�j
	~Wall(); // Wall�̒��_���̃����[�X�i�f�X�g���N�^�j

	void Draw_Wall(); // Wall�̕`��
	void Set_Wall(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Wall�̃e�N�X�`���E�ʒu�E��]�E�傫���̐ݒ�
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Wall�̌��]�t���O�E���]���a�E���]���x�̐ݒ�
	float Rotation_Correction(float r); // Wall�̉�]�̕␳
};

void Wall_Initialize(); // Wall�̏�����
void Wall_Finalize(); // Wall�̏I������
void Wall_Update(); // Wall�̍X�V
void Wall_Draw(); // Wall�̕`��
#endif