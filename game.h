#include "bg.h"
#include "player.h"
#include "arrow.h"

#ifndef GAME_H_
#define GAME_H_

void Game_Initialize(); // Gameの初期化
void Game_Finalize(); // Gameの終了処理
void Game_Update(); // Gameの更新
void Game_Draw(); // Gameの描画
#endif