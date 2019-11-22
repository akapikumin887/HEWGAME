#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "common.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "sprite.h"
#include "texture.h"

// グローバル変数
static D3DCOLOR g_Color = 0xffffffff; // 白不透明
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_mtxWorld; // ワールドマトリックス
static D3DXMATRIX g_mtxGWorld;
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBufferCube = NULL; // Cube頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBufferFace = NULL; // Face頂点バッファ
static LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBufferFace = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffer = NULL;

//====================================================
// 初期化
//====================================================
void Sprite_Initialize_2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	// バーテックスバッファを作成する
	pDevice->CreateVertexBuffer(sizeof(Vertex_2D) * 4, // 4頂点を作る
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_DEFAULT,
		&g_pVertexBuffer,
		NULL);

	// インデックスバッファを使う
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, // インデックス6こぶん
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIndexBuffer,
		NULL);

	// インデックスに書き込んでおく
	LPWORD pIndex = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0); // インデックス配列のポインタをもらう
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;
	g_pIndexBuffer->Unlock(); // 描き込み終了
}

//====================================================
// スプライト表示
//====================================================
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx,int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// テクスチャ座標計算
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx+tw) / w;
	v[1] = (float)(ty + th) / h; // ピクセル座標をテクスチャ座標に変換

	Vertex_2D* pv; // 頂点配列のポインタの入れ物

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // 配列のポインタを受け取る

	// 頂点データを設定
	pv[0].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}
	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock(); // バーテックスバッファへの書き込み終了

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex_2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス
	
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // 頂点数
		0, 2); // 2がポリゴンの数
}

void Sprite_Draw_Rotation_2D(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// 行列変数宣言
	D3DXMATRIX world; // ワールド行列
	D3DXMATRIX rotation; // 回転行列
	D3DXMATRIX translation; // 平行移動行列

	D3DXMatrixTranslation(&translation, dx, dy, 0.0f); // 平行移動行列作成
	D3DXMatrixRotationZ(&rotation, radian); // radianだけｚ軸回転する行列を作成
	D3DXMatrixMultiply(&world, &rotation, &translation); // world = rotation * translation

	// テクスチャ座標計算
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h; // ピクセル座標をテクスチャ座標に変換
	
	Vertex_2D* pv;

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // 配列のポインタを受け取る

	// 頂点データを設定
	pv[0].position = D3DXVECTOR4(
		-lx - 0.5f,
		-ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		+lx - 0.5f,
		-ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		-lx - 0.5f,
		+ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		+lx - 0.5f,
		+ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}

	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	// 頂点座標を変換する
	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(
			&pv[i].position, // 変換結果の格納先ポインタ
			&pv[i].position, // 変換前データのポインタ
			&world); // 変換に使う行列のポインタ
	}

	g_pVertexBuffer->Unlock(); // バーテックスバッファへの書き込み終了

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex_2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(Vertex_2D));
}

//====================================================
// BG用スプライト描画
// 　tx, ty テクスチャ左上座標　　tw, th 表示スプライトサイズ
//====================================================
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}

	float w = (float)Texture_GetWidth(texture_index);//テクスチャサイズ
	float h = (float)Texture_GetHeight(texture_index);

	// テクスチャ座標計算
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = u[0] + 1.0f;
	v[1] = v[0] + 1.0f; // ピクセル座標をテクスチャ座標に変換

	Vertex_2D* pv; // 頂点配列のポインタの入れ物

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // 配列のポインタを受け取る

    // 頂点データを設定
	pv[0].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}
	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock(); // バーテックスバッファへの書き込み終了

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex_2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // 頂点数
		0, 2); // 2がポリゴンの数
}

void Sprite_SetColor_2D(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Finalize_2D(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void Sprite_Finalize_3D()
{
	if (g_pD3DVtxBufferFace)
	{
		g_pD3DVtxBufferFace->Release();
		g_pD3DVtxBufferFace = NULL;
	}

	if (g_pD3DVtxBufferCube)
	{
		g_pD3DVtxBufferCube->Release();
		g_pD3DVtxBufferCube = NULL;
	}

	if (g_pD3DIdxBufferFace != NULL)
	{
		g_pD3DIdxBufferFace->Release(); // インターフェースの解放
		g_pD3DIdxBufferFace = NULL;
	}
}

// Face（バッファ）の生成
void CreateFace()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // 頂点データ用に確保するバッファサイズ（バイト単位）
		D3DUSAGE_WRITEONLY,   // 頂点バッファの使用法
		FVF_VERTEX_3D,        // 使用する頂点フォーマット
		D3DPOOL_MANAGED,      // リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBufferFace, // 頂点バッファインターフェースへのポインタ
		NULL);                // NULLに設定

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBufferFace->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, 0.0f, 0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-0.5f, 0.0f, -0.5f);
	pVtx[3].vtx = D3DXVECTOR3(0.5f, 0.0f, -0.5f);
	
	for (int i = 0; i < 4; i++)
	{
		// 法線ベクトルの設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//反射光の設定
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	g_pD3DVtxBufferFace->Unlock();

	// ストリップ用
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pD3DIdxBufferFace,
		NULL);

	WORD* pIdx = NULL; // 配列の先頭ポインタの入れ物

	g_pD3DIdxBufferFace->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 3;

	g_pD3DIdxBufferFace->Unlock();
}

// Cube（バッファ）の生成
void CreateCube()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 36, // 頂点データ用に確保するバッファサイズ（バイト単位）
		D3DUSAGE_WRITEONLY,   // 頂点バッファの使用法
		FVF_VERTEX_3D,        // 使用する頂点フォーマット
		D3DPOOL_MANAGED,      // リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBufferCube, // 頂点バッファインターフェースへのポインタ
		NULL);                // NULLに設定

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBufferCube->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	pVtx[3].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[4].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx[5].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	pVtx[6].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[7].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[8].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);

	pVtx[9].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[10].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[11].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);

	pVtx[12].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[13].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[14].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	pVtx[15].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[16].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx[17].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	pVtx[18].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[19].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[20].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);

	pVtx[21].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[22].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVtx[23].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);

	pVtx[24].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[25].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx[26].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	pVtx[27].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx[28].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx[29].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	pVtx[30].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx[31].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx[32].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	pVtx[33].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx[34].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx[35].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	for (int i = 0; i < 36; i++)
	{
		// 法線ベクトルの設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//反射光の設定
		pVtx[i].diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	}

	for (int i = 0; i < 36; i += 6)
	{
		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[i + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[i + 3].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 4].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[i + 5].tex = D3DXVECTOR2(0.0f, 1.0f);
	}
	//頂点データをアンロックする
	g_pD3DVtxBufferCube->Unlock();
}

// Faceの描画
void VERTEX_3D::Sprite_Draw_Face(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxScl; // スケーリング行列
	D3DXMATRIX mtxRot; // 回転行列
	D3DXMATRIX mtxTrs; // 平行移動行列

	D3DXMatrixIdentity(&g_mtxWorld); // ワールド行列を単位行列に初期化

	// スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl); // World * Scaling

	// 回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot); // World * Rotation

	// 公転
	if (Revolution)
	{
		// 平行移動行列を作成＆ワールド行列へ合成
		D3DXMatrixTranslation(&mtxTrs, RevRadius.x, RevRadius.y, RevRadius.z);
		D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTrs); // World * Translation

		// 回転行列を作成＆ワールド行列へ合成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(RevSpd.y), D3DXToRadian(RevSpd.x), D3DXToRadian(RevSpd.z));
		D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot); // World * Rotation
	}

	// 平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTrs); // World * Translation

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	// 描画したいポリゴンの頂点バッファをデータストリーム（データの通り道）にセット
	pDevice->SetStreamSource(0, g_pD3DVtxBufferFace, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(g_pD3DIdxBufferFace);

	// 描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

// Cubeの描画
void VERTEX_3D::Sprite_Draw_Cube(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxScl; // スケーリング行列
	D3DXMATRIX mtxRot; // 回転行列
	D3DXMATRIX mtxTrs; // 平行移動行列

	D3DXMatrixIdentity(&g_mtxWorld); // ワールド行列を単位行列に初期化

	// スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl); // World * Scaling

	// 回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot); // World * Rotation

	// 公転
	if (Revolution)
	{
		// 平行移動行列を作成＆ワールド行列へ合成
		D3DXMatrixTranslation(&mtxTrs, RevRadius.x, RevRadius.y, RevRadius.z);
		D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTrs); // World * Translation

		// 回転行列を作成＆ワールド行列へ合成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(RevSpd.y), D3DXToRadian(RevSpd.x), D3DXToRadian(RevSpd.z));
		D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot); // World * Rotation
	}

	// 平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTrs); // World * Translation

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	// 描画したいポリゴンの頂点バッファをデータストリーム（データの通り道）にセット
	pDevice->SetStreamSource(0, g_pD3DVtxBufferCube, 0, sizeof(VERTEX_3D));

	// 描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);
}

// 回転角度の補正
float VERTEX_3D::Rotation_Correction(float r)
{
	if (r > 360.0f)
	{
		r -= 360.0f;
	}
	if (r < 0.0f)
	{
		r += 360.0f;
	}
	return r;
}