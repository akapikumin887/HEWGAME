#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "sprite.h"
#include "texture.h"

#ifndef XMODEL_H_
#define XMODEL_H_

void XModel_Initialize(); // XModel�̏�����
void XModel_Finalize(); // XModel�̏I������
void XModel_Update(); // XModel�̍X�V
void XModel_Draw(); // XModel�̕`��
#endif