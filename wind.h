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

#ifndef Wind_H_
#define Wind_H_

class Wind
{
public:
	D3DXVECTOR3 WindSpeed;     // 風速
	bool wright;               // 右風向フラグ
	bool wleft;               // 左風向フラグ

	Wind();
	~Wind();
};

void Wind_Initialize(); // Windの初期化
void Wind_Finalize(); // Windの終了処理
void Wind_Update(); // Windの更新
void Wind_Draw(); // Windの描画
Wind* GetWind(); // Windの情報の取得
#endif