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

#ifndef Score_H_
#define Score_H_

void Score_Initialize(); // Score�̏�����
void Score_Finalize(); // Score�̏I������
void Score_Update(); // Score�̍X�V
void Score_Draw(); // Score�̕`��
void Add_Score(int s); // Score�̉��Z
#endif