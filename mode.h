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

#define MODE_OBJECT_MAX 5

enum MODE {
	MODE_NONE,
	MODE_EASY,
	MODE_NORMAL,
	MODE_HARD,
};

class Mode
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	Face         *face;         // Face

	Mode();  // Mode�̏������i�R���X�g���N�^�j
	~Mode(); // Mode�̏I�������i�f�X�g���N�^�j

	void Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz); // Mode�̏�����
	void Finalize(); // Mode�̏I������
	void Update();   // Mode�̍X�V
	void Draw();     // Mode�̕`��
};

void Mode_Initialize();
void Mode_Finalize();
void Mode_Update();
void Mode_Draw();
MODE Get_Mode();
void Set_Mode(MODE m);