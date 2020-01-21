#pragma once
// DirectX関連の処理をまとめる
#include <windows.h>
#include <d3d9.h>

// DirectXの初期化、終了処理、デバイスの取得
bool Mydirect3D_Initialize(HWND hwnd);
void Mydirect3D_Finalize(void);
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice();