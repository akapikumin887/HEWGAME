#include "game.h"
#include "input.h"

// Game�̏�����
void Game_Initialize()
{
	Player_Initialize();
}

// Game�̏I������
void Game_Finalize()
{
	Player_Finalize();
}

// Game�̍X�V
void Game_Update()
{
	Player_Update();
}

// Game�̕`��
void Game_Draw()
{
	Player_Draw();
}