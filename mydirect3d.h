#pragma once
// DirectX�֘A�̏������܂Ƃ߂�
#include <windows.h>
#include <d3d9.h>

// DirectX�̏������A�I�������A�f�o�C�X�̎擾
bool Mydirect3D_Initialize(HWND hwnd);
void Mydirect3D_Finalize(void);
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice();