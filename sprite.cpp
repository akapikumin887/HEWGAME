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
#include "camera.h"

// グローバル変数
static D3DCOLOR g_Color = 0xffffffff; // 白不透明
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_mtxGWorld;

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
	u[1] = (float)(tx + tw) / w;
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

void Sprite_Draw_Matrix_2D(TextureIndex texture_index, D3DXVECTOR2 s, D3DXVECTOR2 p, int tx, int ty, int tw, int th, float r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// 行列変数宣言
	D3DXMATRIX mtxWorld; // ワールド行列
	D3DXMATRIX mtxScl;   // スケール行列
	D3DXMATRIX mtxRot;   // 回転行列
	D3DXMATRIX mtxTrs;   // 平行移動行列

	D3DXMatrixIdentity(&mtxWorld); // ワールド行列を単位行列に初期化

	// 回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, D3DXToRadian(r));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// 平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, p.x, p.y, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// テクスチャ座標計算
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h; // ピクセル座標をテクスチャ座標に変換
	
	Vertex_2D* pv;

	float	lx = s.x / 2.0f;
	float	ly = s.y / 2.0f;

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
			&mtxWorld);      // 変換に使う行列のポインタ
	}

	g_pVertexBuffer->Unlock(); // バーテックスバッファへの書き込み終了

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer,    // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex_2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(Vertex_2D));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // 頂点数
		0, 2); // 2がポリゴンの数
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

// スケール行列の設定(ワールド行列の初期化を含む)
D3DXMATRIX Set_Mtx_Scl(D3DXMATRIX mtx, D3DXVECTOR3 s)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxScl; // スケーリング行列

	// スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl, s.x, s.y, s.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScl); // World * Scaling

	return mtx;
}

// 回転行列の設定
D3DXMATRIX Set_Mtx_Rot(D3DXMATRIX mtx, D3DXVECTOR3 r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxRot; // 回転行列

	// 回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(r.y), D3DXToRadian(r.x), D3DXToRadian(r.z));
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot); // World * Rotation

	return mtx;
}

// 平行移動行列の設定
D3DXMATRIX Set_Mtx_Trs(D3DXMATRIX mtx, D3DXVECTOR3 p)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxTrs; // 平行移動行列

	// 平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, p.x, p.y, p.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrs); // World * Translation

	return mtx;
}

// Billboard（バッファ）の生成
D3DXVECTOR3 CreateBillboard(D3DXVECTOR3 sz, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // 頂点データ用に確保するバッファサイズ（バイト単位）
		D3DUSAGE_WRITEONLY, // 頂点バッファの使用法
		FVF_VERTEX_3D,      // 使用する頂点フォーマット
		D3DPOOL_MANAGED,    // リソースのバッファを保持するメモリクラスを指定
		&vb,                // 頂点バッファインターフェースへのポインタ
		NULL);              // NULLに設定

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	vb->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-sz.x / 2, sz.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(sz.x / 2, sz.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-sz.x / 2, -sz.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(sz.x / 2, -sz.y / 2, 0.0f);

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
	vb->Unlock();

	// ストリップ用
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&ib,
		NULL);

	WORD* pIdx = NULL; // 配列の先頭ポインタの入れ物

	ib->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < 4; i++)
	{
		pIdx[i] = i;
	}

	ib->Unlock();

	return sz;
}

// Billboardの描画時のCamera回転のセット
D3DXMATRIX Set_Biillboard_Camera(D3DXMATRIX mtx)
{
	mtx._41 = 0.0f; // Cameraの平行移動行列を0に
	mtx._42 = 0.0f;
	mtx._43 = 0.0f;
	D3DXMatrixTranspose(&mtx, &mtx); // Camera行列の転置

	return mtx;
}

// Billboardの描画
void Draw_Billboard(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXMATRIX &mtxW, D3DXMATRIX mtxC, LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DINDEXBUFFER9 ib)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxScl;    // スケーリング行列
	D3DXMATRIX mtxCamera; // 回転行列(Camera転置)
	D3DXMATRIX mtxTrs;    // 平行移動行列

	D3DXMatrixIdentity(&mtxW); // ワールド行列を単位行列に初期化

	// スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl, 1, 1, 1);
	D3DXMatrixMultiply(&mtxW, &mtxW, &mtxScl); // World * Scaling

	// 回転行列を作成＆ワールド行列へ合成
	mtxCamera = mtxC;
	mtxCamera._41 = 0.0f; // Cameraの平行移動行列を0に
	mtxCamera._42 = 0.0f;
	mtxCamera._43 = 0.0f;
	D3DXMatrixTranspose(&mtxCamera, &mtxCamera); // Camera行列の転置
	D3DXMatrixMultiply(&mtxW, &mtxW, &mtxCamera); // World * Rotation

	// 平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, p.x, p.y, p.z);
	D3DXMatrixMultiply(&mtxW, &mtxW, &mtxTrs); // World * Translation

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxW);

	// 描画したいポリゴンの頂点バッファをデータストリーム（データの通り道）にセット
	pDevice->SetStreamSource(0, vb, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(ib);

	// 描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

// Face
// Faceの初期化（コンストラクタ）
Face::Face()
{
	pD3DVtxBuffer = NULL;
	pD3DIdxBuffer = NULL;
}

// Faceの終了処理（デストラクタ）
Face::~Face()
{
	if (pD3DVtxBuffer)
	{
		pD3DVtxBuffer->Release();
		pD3DVtxBuffer = NULL;
	}

	if (pD3DIdxBuffer)
	{
		pD3DIdxBuffer->Release();
		pD3DIdxBuffer = NULL;
	}
}

// Face（バッファ）の生成
void Face::CreateFace(D3DXVECTOR3 sz)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // 頂点データ用に確保するバッファサイズ（バイト単位）
		D3DUSAGE_WRITEONLY,   // 頂点バッファの使用法
		FVF_VERTEX_3D,        // 使用する頂点フォーマット
		D3DPOOL_MANAGED,      // リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffer, // 頂点バッファインターフェースへのポインタ
		NULL);                // NULLに設定

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-sz.x / 2, sz.y, sz.z / 2);
	pVtx[1].vtx = D3DXVECTOR3(sz.x / 2, sz.y, sz.z / 2);
	pVtx[2].vtx = D3DXVECTOR3(-sz.x / 2, sz.y, -sz.z / 2);
	pVtx[3].vtx = D3DXVECTOR3(sz.x / 2, sz.y, -sz.z / 2);

	for (int i = 0; i < 4; i++)
	{
		// 法線ベクトルの設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//反射光の設定
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	pD3DVtxBuffer->Unlock();

	// ストリップ用
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBuffer,
		NULL);

	WORD* pIdx = NULL; // 配列の先頭ポインタの入れ物

	pD3DIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < 4; i++)
	{
		pIdx[i] = i;
	}

	pD3DIdxBuffer->Unlock();
}

// Faceの描画
void Face::Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// 描画したいポリゴンの頂点バッファをデータストリーム（データの通り道）にセット
	pDevice->SetStreamSource(0, pD3DVtxBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBuffer);

	// 描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

// FaceEX
// FaceEXの初期化（コンストラクタ）
FaceEX::FaceEX()
{
	pD3DVtxBufferEX = NULL;
	pD3DIdxBufferEX = NULL;
}

// FaceEXの終了処理（デストラクタ）
FaceEX::~FaceEX()
{
	if (pD3DVtxBufferEX)
	{
		pD3DVtxBufferEX->Release();
		pD3DVtxBufferEX = NULL;
	}

	if (pD3DIdxBufferEX)
	{
		pD3DIdxBufferEX->Release();
		pD3DIdxBufferEX = NULL;
	}
}

// FaceEX（バッファ）の生成(縮退ポリゴン)
void FaceEX::CreateFaceEX(D3DXVECTOR3 sz, D3DXVECTOR3 szn)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	kx = (int)(sz.x / szn.x); // 横段数
	kz = (int)(sz.z / szn.z); // 縦段数

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((kx + 1) * (kz + 1)), // 頂点データ用に確保するバッファサイズ（バイト単位）
		D3DUSAGE_WRITEONLY, // 頂点バッファの使用法
		FVF_VERTEX_3D,      // 使用する頂点フォーマット
		D3DPOOL_MANAGED,    // リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBufferEX,   // 頂点バッファインターフェースへのポインタ
		NULL);              // NULLに設定

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBufferEX->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < (kx + 1) * (kz + 1); i++)
	{
		// 頂点座標の設定
		pVtx[i].vtx = D3DXVECTOR3((i % (kx + 1)) * szn.x - sz.x / 2.0f, sz.y, sz.z / 2.0f - i / (kx + 1) * szn.z);

		// 法線ベクトルの設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//反射光の設定
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		//pVtx[i].tex = D3DXVECTOR2((float)(i % (kx + 1)), (float)(i / (kx + 1))); // マスごとにテクスチャを貼る
		pVtx[i].tex = D3DXVECTOR2((float)(i % (kx + 1)) / kx, (float)(i / (kx + 1)) / kz); // 全体にテクスチャを貼る
	}

	//頂点データをアンロックする
	pD3DVtxBufferEX->Unlock();

	// ストリップ用
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((3 + (kz * 2) - 1) * kx + 2 * (kx - 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBufferEX,
		NULL);

	WORD* pIdx = NULL; // 配列の先頭ポインタの入れ物

	pD3DIdxBufferEX->Lock(0, 0, (void**)&pIdx, 0);

	int count = 0;

	for (int i = 0; i < kx; i++)
	{
		for (int j = 0; j < kz + 1; j++)
		{
			pIdx[count] = i + j * (kx + 1);
			pIdx[count + 1] = pIdx[count] + 1;
			count += 2;
		}
		pIdx[count] = pIdx[count - 1];
		pIdx[count + 1] = pIdx[count - (kz + 1) * 2 + 1];
	}

	pD3DIdxBufferEX->Unlock();
}

// FaceEXの描画
void FaceEX::Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// 描画したいポリゴンの頂点バッファをデータストリーム（データの通り道）にセット
	pDevice->SetStreamSource(0, pD3DVtxBufferEX, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBufferEX);

	// 描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ((3 + (kz * 2) - 1) * kx + 2 * (kx - 1)), 0, kz * kx * 2 + (kx - 1) * 2);
}

// Cube
// Cubeの初期化（コンストラクタ）
Cube::Cube()
{
	pD3DVtxBuffer = NULL;
	pD3DIdxBuffer = NULL;
}

// Cubeの終了処理（デストラクタ）
Cube::~Cube()
{
	if (pD3DVtxBuffer)
	{
		pD3DVtxBuffer->Release();
		pD3DVtxBuffer = NULL;
	}

	if (pD3DIdxBuffer)
	{
		pD3DIdxBuffer->Release();
		pD3DIdxBuffer = NULL;
	}
}

// Cube（バッファ）の生成
void Cube::CreateCube(D3DXVECTOR3 sz)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXVECTOR3 s = sz / 2;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 36, // 頂点データ用に確保するバッファサイズ（バイト単位）
		D3DUSAGE_WRITEONLY, // 頂点バッファの使用法
		FVF_VERTEX_3D,      // 使用する頂点フォーマット
		D3DPOOL_MANAGED,    // リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffer,     // 頂点バッファインターフェースへのポインタ
		NULL);              // NULLに設定

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-s.x, s.y, s.z);
	pVtx[1].vtx = D3DXVECTOR3(s.x, s.y, s.z);
	pVtx[2].vtx = D3DXVECTOR3(-s.x, s.y, -s.z);

	pVtx[3].vtx = pVtx[1].vtx;
	pVtx[4].vtx = D3DXVECTOR3(s.x, s.y, -s.z);
	pVtx[5].vtx = pVtx[2].vtx;

	pVtx[6].vtx = pVtx[3].vtx;
	pVtx[7].vtx = D3DXVECTOR3(s.x, -s.y, s.z);
	pVtx[8].vtx = pVtx[4].vtx;

	pVtx[9].vtx = pVtx[7].vtx;
	pVtx[10].vtx = D3DXVECTOR3(s.x, -s.y, -s.z);
	pVtx[11].vtx = pVtx[8].vtx;

	pVtx[12].vtx = pVtx[9].vtx;
	pVtx[13].vtx = D3DXVECTOR3(-s.x, -s.y, s.z);
	pVtx[14].vtx = pVtx[10].vtx;

	pVtx[15].vtx = pVtx[13].vtx;
	pVtx[16].vtx = D3DXVECTOR3(-s.x, -s.y, -s.z);
	pVtx[17].vtx = pVtx[14].vtx;

	pVtx[18].vtx = pVtx[15].vtx;
	pVtx[19].vtx = pVtx[0].vtx;
	pVtx[20].vtx = pVtx[16].vtx;

	pVtx[21].vtx = pVtx[19].vtx;
	pVtx[22].vtx = pVtx[5].vtx;
	pVtx[23].vtx = pVtx[20].vtx;

	pVtx[24].vtx = pVtx[6].vtx;
	pVtx[25].vtx = pVtx[21].vtx;
	pVtx[26].vtx = pVtx[12].vtx;

	pVtx[27].vtx = pVtx[25].vtx;
	pVtx[28].vtx = pVtx[15].vtx;
	pVtx[29].vtx = pVtx[26].vtx;

	pVtx[30].vtx = pVtx[22].vtx;
	pVtx[31].vtx = pVtx[11].vtx;
	pVtx[32].vtx = pVtx[23].vtx;

	pVtx[33].vtx = pVtx[31].vtx;
	pVtx[34].vtx = pVtx[17].vtx;
	pVtx[35].vtx = pVtx[32].vtx;

	// 法線ベクトルの設定
	for (int i = 0; i < 6; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	for (int i = 6; i < 12; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	for (int i = 12; i < 18; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}
	for (int i = 18; i < 24; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	for (int i = 24; i < 30; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	for (int i = 30; i < 36; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	}

	//反射光の設定
	for (int i = 0; i < 36; i++)
	{
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標の設定
	for (int i = 0; i < 36; i += 6)
	{
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[i + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[i + 3].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 4].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[i + 5].tex = D3DXVECTOR2(0.0f, 1.0f);
	}

	//頂点データをアンロックする
	pD3DVtxBuffer->Unlock();

	// リスト用
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBuffer,
		NULL);

	WORD* pIdx = NULL; // 配列の先頭ポインタの入れ物

	pD3DIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < 36; i++)
	{
		pIdx[i] = i;
	}

	pD3DIdxBuffer->Unlock();
}

// Cubeの描画
void Cube::Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// 描画したいポリゴンの頂点バッファをデータストリーム（データの通り道）にセット
	pDevice->SetStreamSource(0, pD3DVtxBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBuffer);

	// 描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12);
}