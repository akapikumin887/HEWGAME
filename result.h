#pragma once
#include "common.h"
#include "mydirect3d.h"
#include "arrow.h"

#define STRING_POS_X 300.0f
#define STRING_POS_Y 300.0f
#define BOARD_POS_X 750.0f
#define BOARD_POS_Y 400.0f

void Result_Initialize(); // Result�̏�����
void Result_Finalize();   // Result�̏I������
void Result_Update();     // Result�̍X�V
void Result_Draw();       // Result�̕`��