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

#ifndef Wind_H_
#define Wind_H_

class Wind
{
public:
	D3DXVECTOR3 WindSpeed;     // ����
	bool wright;               // �E�����t���O
	bool wleft;               // �������t���O

	Wind();
	~Wind();
};

void Wind_Initialize(); // Wind�̏�����
void Wind_Finalize(); // Wind�̏I������
void Wind_Update(); // Wind�̍X�V
void Wind_Draw(); // Wind�̕`��
Wind* GetWind(); // Wind�̏��̎擾
#endif