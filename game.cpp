#include "game.h"

// Game‚Ì‰Šú‰»
void Game_Initialize()
{
	BG_Initialize();
	Player_Initialize();
	Arrow_Initialize();
	Target_Initialize();
	Wind_Initialize();
	Score_Initialize();
}

// Game‚ÌI—¹ˆ—
void Game_Finalize()
{
	BG_Finalize();
	Player_Finalize();
	Arrow_Finalize();
	Target_Finalize();
	Wind_Finalize();
}

// Game‚ÌXV
void Game_Update()
{
	BG_Update();
	Player_Update();
	Arrow_Update();
	Target_Update();
	Wind_Update();
	Score_Update();
}

// Game‚Ì•`‰æ
void Game_Draw()
{
	BG_Draw();
	Player_Draw();
	Arrow_Draw();
	Target_Draw();
	Wind_Draw();
	Score_Draw();
}