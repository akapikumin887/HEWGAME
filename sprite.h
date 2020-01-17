#pragma once
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
	D3DXVECTOR3 vtx;     // 頂点座標
	D3DXVECTOR3 nor;     // 法線ベクトル
	D3DCOLOR    diffuse; // 反射光
	D3DXVECTOR2 tex;     // テクスチャ座標
	D3DXMATRIX mtxWorld; // ワールドマトリックス
};

class Face
{
public:
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffer; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffer; // 頂点インデックス

	Face();  // Faceの初期化（コンストラクタ）
	~Face(); // Faceの終了処理（デストラクタ）

	void CreateFace(D3DXVECTOR3 sz); // Face（バッファ）の生成
	void Draw(TextureIndex tex_idx); // Faceの描画
};

class FaceEX
{
public:
	int kx, kz; // 横・縦の段数

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBufferEX; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBufferEX; // 頂点インデックス

	FaceEX();  // FaceEXの初期化（コンストラクタ）
	~FaceEX(); // FaceEXの終了処理（デストラクタ）

	void CreateFaceEX(D3DXVECTOR3 sz, D3DXVECTOR3 szn); // FaceEX（バッファ）の生成(縮退ポリゴン)
	void Draw(TextureIndex tex_idx);                    // FaceEXの描画
};

class Cube
{
public:
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffer; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffer; // 頂点インデックス

	Cube();  // Cubeの初期化（コンストラクタ）
	~Cube(); // Cubeの終了処理（デストラクタ）

	void CreateCube(D3DXVECTOR3 sz); // Cube（バッファ）の生成
	void Draw(TextureIndex tex_idx); // Cubeの描画
};

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 2Dポリゴン頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 3Dポリゴン頂点フォーマット（頂点座標[3D] / 法線 / 反射光 / テクスチャ座標）

//プロトタイプ宣言
void Sprite_Initialize_2D();
void Sprite_Finalize_2D();
void Sprite_SetColor_2D(D3DCOLOR color);//頂点色のセット
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx, int ty,int tw, int th); // dxは表示座標txは左上テクスチャ座標twテクスチャ貼り付けサイズ
void Sprite_Draw_Matrix_2D(TextureIndex texture_index, D3DXVECTOR2 s, D3DXVECTOR2 p, int tx, int ty, int tw, int th, float d = 0.0f); // マトリックスを使った描画
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th); // dxは表示座標txは左上テクスチャ座標twテクスチャ貼り付けサイズ

D3DXMATRIX Set_Mtx_Scl(D3DXMATRIX mtx, D3DXVECTOR3 s = D3DXVECTOR3(1.0f, 1.0f, 1.0f)); // スケール行列の設定(ワールド行列の初期化を含む)
D3DXMATRIX Set_Mtx_Rot(D3DXMATRIX mtx, D3DXVECTOR3 r); // 回転行列の設定
D3DXMATRIX Set_Mtx_Trs(D3DXMATRIX mtx, D3DXVECTOR3 p); // 平行移動行列の設定

D3DXVECTOR3 CreateBillboard(D3DXVECTOR3 sz, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib); // Billboard（バッファ）の生成
D3DXMATRIX Set_Biillboard_Camera(D3DXMATRIX mtx); // Billboardの描画時のCamera回転のセット
void Draw_Billboard(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXMATRIX &mtxW, D3DXMATRIX mtxR, LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DINDEXBUFFER9 ib); // Billboardの描画
#endif 