#include "bg.h"
#include "player.h"
#include "arrow.h"
#include "target.h"
#include "score.h"

#ifndef GAME_H_
#define GAME_H_

void Game_Initialize(); // Game�̏�����
void Game_Finalize(); // Game�̏I������
void Game_Update(); // Game�̍X�V
void Game_Draw(); // Game�̕`��
#endif