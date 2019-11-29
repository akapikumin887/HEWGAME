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
#include "ui.h"

#ifndef Score_H_
#define Score_H_

class Score :public Alphabet
{
public:
	char sa[8];
};

void Score_Initialize(); // Scoreの初期化
void Score_Finalize(); // Scoreの終了処理
void Score_Update(); // Scoreの更新
void Score_Draw(); // Scoreの描画
void Add_Score(int s); // Scoreの加算
Number* Get_Score(); // Score情報の取得
#endif