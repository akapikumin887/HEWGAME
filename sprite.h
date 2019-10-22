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

#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 2D�|���S�����_�t�H�[�}�b�g
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 3D�|���S�����_�t�H�[�}�b�g

//�v���g�^�C�v�錾
void Sprite_Initialize(void);
void Sprite_Finalize();
void Sprite_SetColor(D3DCOLOR color);//���_�F�̃Z�b�g

void Sprite_Draw(TextureIndex texture_index,float dx, float dy, int tx, int ty,int tw, int th); // dx�͕\�����Wtx�͍���e�N�X�`�����Wtw�e�N�X�`���\��t���T�C�Y
void Sprite_Draw_Rotation(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian); // ��]�p
void Sprite_DrawEx(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th); // dx�͕\�����Wtx�͍���e�N�X�`�����Wtw�e�N�X�`���\��t���T�C�Y
#endif 