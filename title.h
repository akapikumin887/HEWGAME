#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos‚ðŽg‚¤‚Ì‚É•K—v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

void Title_Initialize(void);
void Title_Finalize(void);
void Title_Update(void);
void Title_Draw(void);