// DirectX�֘A�̏������܂Ƃ߂�
#include<windows.h>
#include<d3d9.h>

#ifndef MYDIRECT3D_H_
#define MYDIRECT3D_H_

bool Mydirect3D_Initialize(HWND hwnd);
void Mydirect3D_Finalize(void);
LPDIRECT3DDEVICE9 Mydirect3D_GetDevice();
#endif // DirectX�̏������A�I�������A�f�o�C�X�̎擾