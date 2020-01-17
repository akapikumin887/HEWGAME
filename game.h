#pragma once
#include "common.h"
#include "mydirect3d.h"
#include "camera.h"
#include "plane.h"
#include "aiming.h"
#include "arrow.h"
#include "target.h"
#include "gravility.h"
#include "wind.h"

#ifndef GAME_H_
#define GAME_H_

void Game_Object_Create_Public(); // 共通GameObjectのバッファ生成
void Game_Initialize();           // Gameの初期化
void Game_Finalize();             // Gameの終了処理
void Game_Update();               // Gameの更新
void Game_Draw();                 // Gameの描画

CameraFP* Get_Game_CameraFP(); // CameraFP情報の取得
Aiming2D* Get_Game_Aiming(); // Aiming3D情報の取得
Target* Get_Game_Target(); // Target情報の取得
#endif