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
	D3DXVECTOR3 vtx;  // ���_���W
	D3DXVECTOR3 nor;  // �@���x�N�g��
	D3DCOLOR diffuse; // ���ˌ�
	D3DXVECTOR2 tex;  // �e�N�X�`�����W

	void Sprite_Draw_Face(TextureIndex texture_index, // �e�N�X�`���̐ݒ�
		D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, // �ʒu�E��]�E�傫���̐ݒ�
		bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd); // ���]�t���O�E���]���a�E���]���x�̐ݒ�
	void Sprite_Draw_Cube(TextureIndex texture_index, // �e�N�X�`���̐ݒ�
		D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, // �ʒu�E��]�E�傫���̐ݒ�
		bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd); // ���]�t���O�E���]���a�E���]���x�̐ݒ�
	float Rotation_Correction(float r); // ��]�p�x�̕␳
};

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 2D�|���S�����_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 3D�|���S�����_�t�H�[�}�b�g�i���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W�j

//�v���g�^�C�v�錾
void Sprite_Initialize_2D();
void Sprite_Finalize_2D();
void Sprite_SetColor_2D(D3DCOLOR color);//���_�F�̃Z�b�g
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx, int ty,int tw, int th); // dx�͕\�����Wtx�͍���e�N�X�`�����Wtw�e�N�X�`���\��t���T�C�Y
void Sprite_Draw_Rotation_2D(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian); // ��]�p
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th); // dx�͕\�����Wtx�͍���e�N�X�`�����Wtw�e�N�X�`���\��t���T�C�Y
void Sprite_Finalize_3D();
void CreateFace(); // Face�i�o�b�t�@�j�̐���
void CreateCube(); // Cube�i�o�b�t�@�j�̐���
void IndexBuffer_Initialize(); // IndexBuffer�̏�����
void Index_Draw(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd); // IndexBuffer�̕`��
#endif 