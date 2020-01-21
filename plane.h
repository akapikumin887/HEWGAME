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

class Plane :public FaceEX
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	FaceEX       *faceEX;       // FaceEX

	Plane();  // Plane�̏������i�R���X�g���N�^�j
	~Plane(); // Plane�̏I�������i�f�X�g���N�^�j

	void Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz, D3DXVECTOR3 szn); // Plane�̏�����
	void Finalize(); // Plane�̏I������
	void Update();   // Plane�̍X�V
	void Draw();     // Plane�̕`��
};