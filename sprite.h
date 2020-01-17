#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"

#ifndef _SPRITE_H_
#define _SPRITE_H_

// 2D�|���S�����_�t�H�[�}�b�g�\���̂��`
class Vertex_2D
{
public:
	D3DXVECTOR4 position; // ���_���W�i���W�ϊ��ςݒ��_�j
	D3DCOLOR color;       // ���_�J���[(32Bit ARGB�w��)
	D3DXVECTOR2 texcoord; // �e�N�X�`�����W
};

// 3D�|���S�����_�t�H�[�}�b�g�\���̂��`
class VERTEX_3D
{
public:
	D3DXVECTOR3 vtx;     // ���_���W
	D3DXVECTOR3 nor;     // �@���x�N�g��
	D3DCOLOR    diffuse; // ���ˌ�
	D3DXVECTOR2 tex;     // �e�N�X�`�����W
	D3DXMATRIX mtxWorld; // ���[���h�}�g���b�N�X
};

class Face
{
public:
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffer; // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffer; // ���_�C���f�b�N�X

	Face();  // Face�̏������i�R���X�g���N�^�j
	~Face(); // Face�̏I�������i�f�X�g���N�^�j

	void CreateFace(D3DXVECTOR3 sz); // Face�i�o�b�t�@�j�̐���
	void Draw(TextureIndex tex_idx); // Face�̕`��
};

class FaceEX
{
public:
	int kx, kz; // ���E�c�̒i��

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBufferEX; // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBufferEX; // ���_�C���f�b�N�X

	FaceEX();  // FaceEX�̏������i�R���X�g���N�^�j
	~FaceEX(); // FaceEX�̏I�������i�f�X�g���N�^�j

	void CreateFaceEX(D3DXVECTOR3 sz, D3DXVECTOR3 szn); // FaceEX�i�o�b�t�@�j�̐���(�k�ރ|���S��)
	void Draw(TextureIndex tex_idx);                    // FaceEX�̕`��
};

class Cube
{
public:
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffer; // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffer; // ���_�C���f�b�N�X

	Cube();  // Cube�̏������i�R���X�g���N�^�j
	~Cube(); // Cube�̏I�������i�f�X�g���N�^�j

	void CreateCube(D3DXVECTOR3 sz); // Cube�i�o�b�t�@�j�̐���
	void Draw(TextureIndex tex_idx); // Cube�̕`��
};

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 2D�|���S�����_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 3D�|���S�����_�t�H�[�}�b�g�i���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W�j

//�v���g�^�C�v�錾
void Sprite_Initialize_2D();
void Sprite_Finalize_2D();
void Sprite_SetColor_2D(D3DCOLOR color);//���_�F�̃Z�b�g
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx, int ty,int tw, int th); // dx�͕\�����Wtx�͍���e�N�X�`�����Wtw�e�N�X�`���\��t���T�C�Y
void Sprite_Draw_Matrix_2D(TextureIndex texture_index, D3DXVECTOR2 s, D3DXVECTOR2 p, int tx, int ty, int tw, int th, float d = 0.0f); // �}�g���b�N�X���g�����`��
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th); // dx�͕\�����Wtx�͍���e�N�X�`�����Wtw�e�N�X�`���\��t���T�C�Y

D3DXMATRIX Set_Mtx_Scl(D3DXMATRIX mtx, D3DXVECTOR3 s = D3DXVECTOR3(1.0f, 1.0f, 1.0f)); // �X�P�[���s��̐ݒ�(���[���h�s��̏��������܂�)
D3DXMATRIX Set_Mtx_Rot(D3DXMATRIX mtx, D3DXVECTOR3 r); // ��]�s��̐ݒ�
D3DXMATRIX Set_Mtx_Trs(D3DXMATRIX mtx, D3DXVECTOR3 p); // ���s�ړ��s��̐ݒ�

D3DXVECTOR3 CreateBillboard(D3DXVECTOR3 sz, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib); // Billboard�i�o�b�t�@�j�̐���
D3DXMATRIX Set_Biillboard_Camera(D3DXMATRIX mtx); // Billboard�̕`�掞��Camera��]�̃Z�b�g
void Draw_Billboard(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXMATRIX &mtxW, D3DXMATRIX mtxR, LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DINDEXBUFFER9 ib); // Billboard�̕`��
#endif 