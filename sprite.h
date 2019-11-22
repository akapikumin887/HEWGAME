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

// 2Dポリゴン頂点フォーマット構造体を定義
class Vertex_2D
{
public:
	D3DXVECTOR4 position; // 頂点座標（座標変換済み頂点）
	D3DCOLOR color;       // 頂点カラー(32Bit ARGB指定)
	D3DXVECTOR2 texcoord; // テクスチャ座標
};

// 3Dポリゴン頂点フォーマット構造体を定義
class VERTEX_3D
{
public:
	D3DXVECTOR3 vtx;  // 頂点座標
	D3DXVECTOR3 nor;  // 法線ベクトル
	D3DCOLOR diffuse; // 反射光
	D3DXVECTOR2 tex;  // テクスチャ座標

	void Sprite_Draw_Face(TextureIndex texture_index, // テクスチャの設定
		D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, // 位置・回転・大きさの設定
		bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd); // 公転フラグ・公転半径・公転速度の設定
	void Sprite_Draw_Cube(TextureIndex texture_index, // テクスチャの設定
		D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, // 位置・回転・大きさの設定
		bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd); // 公転フラグ・公転半径・公転速度の設定
	float Rotation_Correction(float r); // 回転角度の補正
};

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 2Dポリゴン頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 3Dポリゴン頂点フォーマット（頂点座標[3D] / 法線 / 反射光 / テクスチャ座標）

//プロトタイプ宣言
void Sprite_Initialize_2D();
void Sprite_Finalize_2D();
void Sprite_SetColor_2D(D3DCOLOR color);//頂点色のセット
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx, int ty,int tw, int th); // dxは表示座標txは左上テクスチャ座標twテクスチャ貼り付けサイズ
void Sprite_Draw_Rotation_2D(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian); // 回転用
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th); // dxは表示座標txは左上テクスチャ座標twテクスチャ貼り付けサイズ
void Sprite_Finalize_3D();
void CreateFace(); // Face（バッファ）の生成
void CreateCube(); // Cube（バッファ）の生成
void IndexBuffer_Initialize(); // IndexBufferの初期化
void Index_Draw(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd); // IndexBufferの描画
#endif 