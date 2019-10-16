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

// 2Dポリゴンの頂点構造体
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position; // 頂点座標（座標変換済み頂点）
	D3DCOLOR color; // 頂点カラー(32Bit ARGB指定)
	D3DXVECTOR2 texcoord; // テクスチャ座標
}Vertex2D;

// 3Dポリゴンの頂点構造体
typedef struct Vertex3D_tag
{
	D3DXVECTOR3 vtx; // 頂点座標
	D3DXVECTOR3 nor; // 法線
	D3DXCOLOR diffuse; // 反射光（頂点色）
	D3DXVECTOR2 tex; // テクスチャ座標
}VERTEX_3D;

// グローバル変数
static D3DCOLOR g_Color = 0xffffffff; // 白不透明
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

//====================================================
// 初期化
//====================================================
void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	// バーテックスバッファを作成する
	pDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4, // 4頂点を作る
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX2D,
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
void Sprite_Draw(TextureIndex texture_index,float dx, float dy, int tx,int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

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

	Vertex2D* pv; // 頂点配列のポインタの入れ物

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

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス
	
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // 頂点数
		0, 2); // 2がポリゴンの数
}

void Sprite_Draw_Rotation(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

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
	
	Vertex2D* pv;

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

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(Vertex2D));
}

//====================================================
// BG用スプライト描画
// 　tx, ty テクスチャ左上座標　　tw, th 表示スプライトサイズ
//====================================================
void Sprite_DrawEx(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

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

	Vertex2D* pv; // 頂点配列のポインタの入れ物

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

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // 描画したいポリゴンのバーテックスバッファ
		0,
		sizeof(Vertex2D)); // 頂点1このサイズ

	pDevice->SetIndices(g_pIndexBuffer); // 描画したいポリゴンのインデックス

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // 頂点数
		0, 2); // 2がポリゴンの数
}

void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Finalize(void)
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