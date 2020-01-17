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

void Game_Object_Create_Public(); // ����GameObject�̃o�b�t�@����
void Game_Initialize();           // Game�̏�����
void Game_Finalize();             // Game�̏I������
void Game_Update();               // Game�̍X�V
void Game_Draw();                 // Game�̕`��

CameraFP* Get_Game_CameraFP(); // CameraFP���̎擾
Aiming2D* Get_Game_Aiming(); // Aiming3D���̎擾
Target* Get_Game_Target(); // Target���̎擾
#endif