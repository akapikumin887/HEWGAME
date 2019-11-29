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

#ifndef WIND_H_
#define WIND_H_

#define WIND_FORCE_MAG (FPS_STANTARD * 10)

class Wind
{
public:
	D3DXVECTOR3 speed; // Wind�̑��x
};

void Wind_Initialize(); // Wind�̏�����
void Wind_Finalize(); // Wind�̏I������
void Wind_Update(); // Wind�̍X�V
void Wind_Draw(); // Wind�̕`��
Wind* Get_Wind(); // Wind���̎擾
#endif