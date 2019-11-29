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

#ifndef GRAVILITY_H_
#define GRAVILITY_H_

#define GRAVILITY -9.8f
#define GRAVILITY_FORCE_MAG (FPS_STANTARD * 10)

class Gravility
{
public:
	D3DXVECTOR3 g; // Gravility
};

void Gravility_Initialize(); // Gravility�̏�����
void Gravility_Finalize(); // Gravility�̏I������
void Gravility_Update(); // Gravility�̍X�V
void Gravility_Draw(); // Gravility�̕`��
Gravility* Get_Gravility(); // Gravility���̎擾
#endif