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

#define WIND_FORCE_MAG (FPS_STANTARD * 3)

class Wind
{
public:
	static D3DXVECTOR3 speed; // Windの速度

	static void Initialize(); // Windの初期化
	static void Finalize(); // Windの終了処理
	static void Update(); // Windの更新
	static void Draw(); // Windの描画
};