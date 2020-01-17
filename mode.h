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

class Mode {
private:
	//�T���v���|���S���̕\�����̂Ɋւ���p�����[�^
	D3DXVECTOR3 g_pos;     //�n�ʂ̈ʒu(�l�p�`�̒������W)
	D3DXVECTOR3 g_rot;     //�n�ʂ̌���(��])
	D3DXVECTOR3 g_scl;     //�n�ʂ̑傫��(�X�P�[��)
	D3DXMATRIX g_mtxworld; //���[���h�}�g���b�N�X

	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff;  //���_�o�b�t�@

	D3DXVECTOR3 g_pos2;     //�n�ʂ̈ʒu(�l�p�`�̒������W)
	D3DXVECTOR3 g_rot2;     //�n�ʂ̌���(��])
	D3DXVECTOR3 g_scl2;     //�n�ʂ̑傫��(�X�P�[��)
	D3DXMATRIX g_mtxworld2; //���[���h�}�g���b�N�X

	D3DXVECTOR3 g_pos3;     //�n�ʂ̈ʒu(�l�p�`�̒������W)
	D3DXVECTOR3 g_rot3;     //�n�ʂ̌���(��])
	D3DXVECTOR3 g_scl3;     //�n�ʂ̑傫��(�X�P�[��)
	D3DXMATRIX g_mtxworld3; //���[���h�}�g���b�N�X

	D3DXVECTOR3 g_pos4;     //�n�ʂ̈ʒu(�l�p�`�̒������W)
	D3DXVECTOR3 g_rot4;     //�n�ʂ̌���(��])
	D3DXVECTOR3 g_scl4;     //�n�ʂ̑傫��(�X�P�[��)
	D3DXMATRIX g_mtxworld4; //���[���h�}�g���b�N�X
	D3DLIGHT9 g_light; //���C�g�\����(���C�g��ɂ����)

//�O���[�o���ϐ��ŃC���^�[�t�F�[�X�̓��ꕨ��p��
	LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuff;


public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
};

void Mode_Initialize();
void Mode_Finalize();
void Mode_Update();
void Mode_Draw();
