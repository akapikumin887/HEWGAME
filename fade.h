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

enum FADE_STATE
{
	FADE_STATE_NONE,
	FADE_STATE_IN,
	FADE_STATE_OUT,
};

void Fade_Initialize(); // Fadeの初期化
void Fade_Finalize(); // Fadeの終了処理
void Fade_Update(); // Fadeの更新
void Fade_Draw(); // Fadeの描画
void Fade(SCENE s);
FADE_STATE Get_Fade_State();