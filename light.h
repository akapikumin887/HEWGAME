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

void Light_Initialize(); // Lightの初期化
void Light_Finalize(); // Lightの終了処理
void Light_Update(); // Lightの更新
void Light_Draw(); // Lightの描画