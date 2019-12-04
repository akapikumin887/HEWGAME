#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "sprite.h"
#include "texture.h"

#ifndef XMODEL_H_
#define XMODEL_H_

void XModel_Initialize(); // XModelの初期化
void XModel_Finalize(); // XModelの終了処理
void XModel_Update(); // XModelの更新
void XModel_Draw(); // XModelの描画
#endif