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

#ifndef LIGHT_H_
#define LIGHT_H_

void Light_Initialize(); // Light�̏�����
void Light_Finalize(); // Light�̏I������
void Light_Update(); // Light�̍X�V
void Light_Draw(); // Light�̕`��
#endif