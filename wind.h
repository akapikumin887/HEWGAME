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

#ifndef WIND_H_
#define WIND_H_

#define WIND_FORCE_MAG (FPS_STANTARD * 10)

class Wind
{
public:
	D3DXVECTOR3 speed; // Windの速度
};

void Wind_Initialize(); // Windの初期化
void Wind_Finalize(); // Windの終了処理
void Wind_Update(); // Windの更新
void Wind_Draw(); // Windの描画
Wind* Get_Wind(); // Wind情報の取得
#endif