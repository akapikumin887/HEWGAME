#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

enum FADE_STATE
{
	FADE_STATE_NONE,
	FADE_STATE_IN,
	FADE_STATE_OUT,
};

void Fade_Initialize(); // Fade�̏�����
void Fade_Finalize(); // Fade�̏I������
void Fade_Update(); // Fade�̍X�V
void Fade_Draw(); // Fade�̕`��
void Fade(SCENE s);
FADE_STATE Get_Fade_State();