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
#include "ui.h"

#ifndef Score_H_
#define Score_H_

class Score
{
public:
	Number   *num;
	Alphabet *alpha;
	int      s[5];

	Score();  // Scoreの初期化（コンストラクタ）
	~Score(); // Scoreの終了処理（デストラクタ）

	void Initialize(); // Scoreの初期化
	void Finalize();   // Scoreの終了処理
	void Update();     // Scoreの更新
	void Draw();       // Scoreの描画

	void Add_Score(int s); // Scoreの加算
};
#endif