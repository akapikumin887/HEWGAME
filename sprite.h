#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"

#ifndef _SPRITE_H_
#define _SPRITE_H_

#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 2Dポリゴン頂点フォーマット
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 3Dポリゴン頂点フォーマット

//プロトタイプ宣言
void Sprite_Initialize(void);
void Sprite_Finalize();
void Sprite_SetColor(D3DCOLOR color);//頂点色のセット

void Sprite_Draw(TextureIndex texture_index,float dx, float dy, int tx, int ty,int tw, int th); // dxは表示座標txは左上テクスチャ座標twテクスチャ貼り付けサイズ
void Sprite_Draw_Rotation(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian); // 回転用
void Sprite_DrawEx(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th); // dxは表示座標txは左上テクスチャ座標twテクスチャ貼り付けサイズ
#endif 