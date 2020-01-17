#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

class Mode {
private:
	//サンプルポリゴンの表示自体に関するパラメータ
	D3DXVECTOR3 g_pos;     //地面の位置(四角形の中央座標)
	D3DXVECTOR3 g_rot;     //地面の向き(回転)
	D3DXVECTOR3 g_scl;     //地面の大きさ(スケール)
	D3DXMATRIX g_mtxworld; //ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff;  //頂点バッファ

	D3DXVECTOR3 g_pos2;     //地面の位置(四角形の中央座標)
	D3DXVECTOR3 g_rot2;     //地面の向き(回転)
	D3DXVECTOR3 g_scl2;     //地面の大きさ(スケール)
	D3DXMATRIX g_mtxworld2; //ワールドマトリックス

	D3DXVECTOR3 g_pos3;     //地面の位置(四角形の中央座標)
	D3DXVECTOR3 g_rot3;     //地面の向き(回転)
	D3DXVECTOR3 g_scl3;     //地面の大きさ(スケール)
	D3DXMATRIX g_mtxworld3; //ワールドマトリックス

	D3DXVECTOR3 g_pos4;     //地面の位置(四角形の中央座標)
	D3DXVECTOR3 g_rot4;     //地面の向き(回転)
	D3DXVECTOR3 g_scl4;     //地面の大きさ(スケール)
	D3DXMATRIX g_mtxworld4; //ワールドマトリックス
	D3DLIGHT9 g_light; //ライト構造体(ライト一つにつき一個)

//グローバル変数でインターフェースの入れ物を用意
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
