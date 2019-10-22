#include "game.h"
#include "input.h"

// Gameの初期化
void Game_Initialize()
{
	Player_Initialize();
}

// Gameの終了処理
void Game_Finalize()
{
	Player_Finalize();
}

// Gameの更新
void Game_Update()
{
	Player_Update();
}

// Gameの描画
void Game_Draw()
{
	Player_Draw();
}