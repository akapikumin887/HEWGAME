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

#ifndef PLANE_H_
#define PLANE_H_

class Plane
{
public:
	VERTEX_3D *planev;          // ���_���
	TextureIndex texture_index; // �e�N�X�`��
	D3DXVECTOR3 size;           // �T�C�Y
	D3DXVECTOR3 size_nor;       // �P�ʃT�C�Y
	D3DXVECTOR3 pos;            // �ʒu
	D3DXVECTOR3 rot;            // ��]
	D3DXVECTOR3 scl;            // �傫��
	bool revolution;            // ���]�t���O
	D3DXVECTOR3 revRadius;      // ���]���a
	D3DXVECTOR3 revSpd;         // ���]���x

	Plane(); // Plane�̒��_���擾�i�R���X�g���N�^�j
	~Plane(); // Plane�̒��_���̃����[�X�i�f�X�g���N�^�j

	void Draw_Plane(); // Plane�̕`��
	void Set_Plane(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Plane�̃e�N�X�`���E�ʒu�E��]�E�傫���̐ݒ�
		D3DXVECTOR3 sz, D3DXVECTOR3 szn, // Plane�̃T�C�Y�E�P�ʃT�C�Y�̐ݒ�
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Plane�̌��]�t���O�E���]���a�E���]���x�̐ݒ�
	float Rotation_Correction(float r); // Plane�̉�]�̕␳
};

void Plane_Initialize(); // Plane�̏�����
void Plane_Finalize(); // Plane�̏I������
void Plane_Update(); // Plane�̍X�V
void Plane_Draw(); // Plane�̕`��
#endif