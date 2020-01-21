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

#define GRAVILITY -9.8f
#define GRAVILITY_FORCE_MAG (FPS_STANTARD * 3)

class Gravility
{
public:
	static D3DXVECTOR3 g; // Gravility

	static void Initialize(); // Gravility�̏�����
	static void Finalize();   // Gravility�̏I������
	static void Update();     // Gravility�̍X�V
	static void Draw();       // Gravility�̕`��
};