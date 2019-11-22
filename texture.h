#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d.h>

#define TEXTURE_FILENAME_MAX (64) // �e�N�X�`���t�@�C���p�X�{�t�@�C���e����������Ȃ���Α��₷

typedef enum {
	TEXTURE_INDEX_KIZUNA,
	TEXTURE_INDEX_AIROU,
	TEXTURE_INDEX_AIMING,
	TEXTURE_INDEX_TARGET,
	TEXTURE_INDEX_MAX
}TextureIndex;

int Texture_Load(void); // �e�N�X�`���̃��[�h
void Texture_Release(void); // �e�N�X�`���̃����[�X
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index); // �e�N�X�`���C���^�[�t�F�[�X�̎擾
int Texture_GetWidth(TextureIndex index); // �摜�T�C�Y�̎擾
int Texture_GetHeight(TextureIndex index); // �摜�T�C�Y�̎擾
#endif