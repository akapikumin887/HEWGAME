#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d.h>

#define TEXTURE_FILENAME_MAX (64) // テクスチャファイルパス＋ファイル各文字数足りなければ増やす

typedef enum {
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_MODE,
	TEXTURE_INDEX_BG01,
	TEXTURE_INDEX_PLAYER,
	TEXTURE_INDEX_ARROW00,
	TEXTURE_INDEX_ARROW01,
	TEXTURE_INDEX_MAX
}TextureIndex;

int Texture_Load(void); // テクスチャのロード
void Texture_Release(void); // テクスチャのリリース
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index); // テクスチャインターフェースの習得
int Texture_GetWidth(TextureIndex index);
int Texture_GetHeight(TextureIndex index); // 画像サイズの習得
#endif