#include "game.h"

// Gameの初期化
void Game_Initialize()
{
	BG_Initialize();
	Player_Initialize();
	Arrow_Initialize();
	Target_Initialize();
	Score_Initialize();
}

// Gameの終了処理
void Game_Finalize()
{
	BG_Finalize();
	Player_Finalize();
	Arrow_Finalize();
	Target_Finalize();
}

// Gameの更新
void Game_Update()
{
	BG_Update();
	Player_Update();
	Arrow_Update();
	Target_Update();
	Score_Update();
}

// Gameの描画
void Game_Draw()
{
	BG_Draw();
	Player_Draw();
	Arrow_Draw();
	Target_Draw();
	Score_Draw();
}