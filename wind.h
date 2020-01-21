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

#define WIND_FORCE_MAG (FPS_STANTARD * 3)

class Wind
{
public:
	static D3DXVECTOR3 speed; // Wind�̑��x

	static void Initialize(); // Wind�̏�����
	static void Finalize(); // Wind�̏I������
	static void Update(); // Wind�̍X�V
	static void Draw(); // Wind�̕`��
};