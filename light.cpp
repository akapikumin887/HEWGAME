#include "light.h"

// グローバル変数
D3DLIGHT9 g_Light; // ライト構造体

// Lightの初期化
void Light_Initialize()
{
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9)); // 構造体を0でクリア（必須）
	g_Light.Type = D3DLIGHT_DIRECTIONAL; // ライトの種類
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 光の色RGBA（0～1）
	g_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 環境光の色

	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.0f, 1.0f, -1.0f); // 光の差す方向ベクトル
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_Light.Direction = vecDir; // 正規化してセット

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetLight(0, &g_Light); // 0番ライトへデータをセット
	pDevice->LightEnable(0, TRUE); // 0番ライトを有効化
}

// Lightの終了処理
void Light_Finalize()
{
	
}

// Lightの更新
void Light_Update()
{
	
}

// Lightの描画
void Light_Draw()
{

}