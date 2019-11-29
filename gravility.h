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

#ifndef GRAVILITY_H_
#define GRAVILITY_H_

#define GRAVILITY -9.8f
#define GRAVILITY_FORCE_MAG (FPS_STANTARD * 10)

class Gravility
{
public:
	D3DXVECTOR3 g; // Gravility
};

void Gravility_Initialize(); // Gravilityの初期化
void Gravility_Finalize(); // Gravilityの終了処理
void Gravility_Update(); // Gravilityの更新
void Gravility_Draw(); // Gravilityの描画
Gravility* Get_Gravility(); // Gravility情報の取得
#endif