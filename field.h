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

#define FIELD_MAX 10

class Field
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	Face         *face;         // Face

	Field();  // Field�̏������i�R���X�g���N�^�j
	~Field(); // Field�̏I�������i�f�X�g���N�^�j

	void Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz); // Field�̏�����
	void Finalize(); // Field�̏I������
	void Update();   // Field�̍X�V
	void Draw();     // Field�̕`��
};