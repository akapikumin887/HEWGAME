#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d.h>

#define TEXTURE_FILENAME_MAX (64) // テクスチャファイルパス＋ファイル各文字数足りなければ増やす

typedef enum {
	TEXTURE_INDEX_KIZUNA,
	TEXTURE_INDEX_AIROU,
	TEXTURE_INDEX_AIMING,
	TEXTURE_INDEX_TARGET,
	TEXTURE_INDEX_MAX
}TextureIndex;

int Texture_Load(void); // テクスチャのロード
void Texture_Release(void); // テクスチャのリリース
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index); // テクスチャインターフェースの取得
int Texture_GetWidth(TextureIndex index); // 画像サイズの取得
int Texture_GetHeight(TextureIndex index); // 画像サイズの取得
#endif