#include "game.h"

// Game�̏�����
void Game_Initialize()
{
	BG_Initialize();
	Player_Initialize();
	Arrow_Initialize();
}

// Game�̏I������
void Game_Finalize()
{
	BG_Finalize();
	Player_Finalize();
	Arrow_Finalize();
}

// Game�̍X�V
void Game_Update()
{
	BG_Update();
	Player_Update();
	Arrow_Update();
}

// Game�̕`��
void Game_Draw()
{
	BG_Draw();
	Player_Draw();
	Arrow_Draw();
}