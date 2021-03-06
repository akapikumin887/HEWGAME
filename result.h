#pragma once
#include "common.h"
#include "mydirect3d.h"
#include "arrow.h"

#define STRING_POS_X 300.0f
#define STRING_POS_Y 300.0f
#define BOARD_POS_X 750.0f
#define BOARD_POS_Y 400.0f

void Result_Initialize(); // Resultの初期化
void Result_Finalize();   // Resultの終了処理
void Result_Update();     // Resultの更新
void Result_Draw();       // Resultの描画