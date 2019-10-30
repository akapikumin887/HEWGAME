#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d.h>

#define TEXTURE_FILENAME_MAX (64) // �e�N�X�`���t�@�C���p�X�{�t�@�C���e����������Ȃ���Α��₷

typedef enum {
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_MODE,
	TEXTURE_INDEX_BG01,
	TEXTURE_INDEX_PLAYER,
	TEXTURE_INDEX_ARROW00,
	TEXTURE_INDEX_ARROW01,
	TEXTURE_INDEX_MAX
}TextureIndex;

int Texture_Load(void); // �e�N�X�`���̃��[�h
void Texture_Release(void); // �e�N�X�`���̃����[�X
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index); // �e�N�X�`���C���^�[�t�F�[�X�̏K��
int Texture_GetWidth(TextureIndex index);
int Texture_GetHeight(TextureIndex index); // �摜�T�C�Y�̏K��
#endif