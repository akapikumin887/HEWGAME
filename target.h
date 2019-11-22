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

#define TARGET_X AIMING_X
#define TARGET_Y 3.5f
#define TARGET_Z AIMING_Z
#define TARGET_MAG_Z AIMING_MAG_Z

class Target
{
public:
	VERTEX_3D *targetv;         // ���_���
	D3DXVECTOR3 pos;            // �ʒu
	D3DXVECTOR3 rot;            // ��]
	D3DXVECTOR3 scl;            // �傫��
	TextureIndex texture_index; // �e�N�X�`��
	bool revolution;            // ���]�t���O
	D3DXVECTOR3 revRadius;      // ���]���a
	D3DXVECTOR3 revSpd;         // ���]���x

	Target(); // Target�̒��_���擾�i�R���X�g���N�^�j
	~Target(); // Target�̒��_���̃����[�X�i�f�X�g���N�^�j

	void Draw_Target(); // Target�̕`��
	void Set_Target(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Target�̃e�N�X�`���E�ʒu�E��]�E�傫���̐ݒ�
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Target�̌��]�t���O�E���]���a�E���]���x�̐ݒ�
	float Rotation_Correction(float r); // Target�̉�]�̕␳
};

void Target_Initialize(); // Target�̏�����
void Target_Finalize(); // Target�̏I������
void Target_Update(); // Target�̍X�V
void Target_Draw(); // Target�̕`��
Target* Get_Target(); // Target���̎擾
#endif