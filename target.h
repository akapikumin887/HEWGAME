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

#define TARGET_X 0.0f
#define TARGET_Y 0.0f
#define TARGET_Z 23.0f
#define TARGET_MAG_Z 6.0f
#define TARGET_SIZE 5.0f

class Target
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	Face         *face;         // Face
	float        circle[11];    // �ւ̐�

	Target();  // Target�̏������i�R���X�g���N�^�j
	~Target(); // Target�̏I�������i�f�X�g���N�^�j

	void Initialize(TextureIndex tex_idx = TEXTURE_INDEX_TARGET, D3DXVECTOR3 p = D3DXVECTOR3(TARGET_X, TARGET_Y, TARGET_Z), D3DXVECTOR3 r = D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3 sz = D3DXVECTOR3(TARGET_SIZE, 0.0f, TARGET_SIZE)); // Target�̏�����
	void Finalize(); // Target�̏I������
	void Update();   // Target�̍X�V
	void Draw();     // Target�̕`��
};