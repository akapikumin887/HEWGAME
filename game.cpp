#include "game.h"
#include "input.h"

// Game‚Ì‰Šú‰»
void Game_Initialize()
{
	Player_Initialize();
}

// Game‚ÌI—¹ˆ—
void Game_Finalize()
{
	Player_Finalize();
}

// Game‚ÌXV
void Game_Update()
{
	Player_Update();
}

// Game‚Ì•`‰æ
void Game_Draw()
{
	Player_Draw();
}