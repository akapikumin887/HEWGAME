#include "game.h"

// Game‚Ì‰Šú‰»
void Game_Initialize()
{
	BG_Initialize();
	Player_Initialize();
	Arrow_Initialize();
}

// Game‚ÌI—¹ˆ—
void Game_Finalize()
{
	BG_Finalize();
	Player_Finalize();
	Arrow_Finalize();
}

// Game‚ÌXV
void Game_Update()
{
	BG_Update();
	Player_Update();
	Arrow_Update();
}

// Game‚Ì•`‰æ
void Game_Draw()
{
	BG_Draw();
	Player_Draw();
	Arrow_Draw();
}