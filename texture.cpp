#include <d3dx9.h>
#include <d3d.h>
#include "mydirect3d.h"
#include "texture.h"
#include "common.h"

#define TEXTURE_FILENAME_MAX (64)

// ファイル管理構造
typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX];
	int width;
	int height;
}TextureFile;

// テクスチャ配列
static const TextureFile g_TextureFiles[] = {
	{ "asset/texture/bg.png", SCREEN_WIDTH, SCREEN_HEIGHT },
	{ "asset/texture/title.png", SCREEN_WIDTH, SCREEN_HEIGHT },
	{ "asset/texture/title_start.png", 512, 110 },
	{ "asset/texture/title_ranking.png", 512, 110 },
	{ "asset/texture/title_arrow.png", 160, 120 },
	{ "asset/texture/mode_easy.jpg", 724, 1024 },
	{ "asset/texture/mode_normal.jpg", 724, 1024 },
	{ "asset/texture/mode_hard.jpg", 724, 1024 },
	{ "asset/texture/mode_select.png", 724, 1024 },
	{ "asset/texture/aiming.png", 128, 128 },
	{ "asset/texture/bow.tga", SCREEN_WIDTH, SCREEN_HEIGHT },
	{ "asset/texture/arrow_bow.jpg", 100, 600 },
	{ "asset/texture/target.png", 512, 512 },
	{ "asset/texture/sky.jpg", 640, 343 },
	{ "asset/texture/plane.jpg", 1080, 1920 },
	{ "asset/texture/side.jpg", 1920, 1080 },
	{ "asset/texture/board.png", 512, 512 },
	{ "asset/texture/mark.png", 64, 64 },
	{ "asset/texture/number.png", 320, 32 },
	{ "asset/texture/alphabet.png", 416, 64 }
	// ヘッダーで定義したenumと対応しているので数が異なったり定義名があらわす画像ファイルの順番が異なったりしないようにする
};

// テクスチャの数
static const int TEXTURE_FILE_COUNT =
sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);

// enumとテクスチャファイルの数が同じか？
static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT,
	"TEXTURE_INDEX_MAX!=TEXTURE_FILE_COUNT");

// グローバル変数
static LPDIRECT3DTEXTURE9
g_pTextures[TEXTURE_FILE_COUNT];

int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)return TEXTURE_FILE_COUNT; // ロードできなかった数を返す

	int failed_count = 0; // ロードできなかった数
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	{
		if ( FAILED( D3DXCreateTextureFromFile(
			pDevice,
			g_TextureFiles[i].filename,
			&g_pTextures[i])))
		{
			// なんかエラー表示
			failed_count++;
		}
	}
	return failed_count;
}

void Texture_Release(void)
{
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	{
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

// インターフェース取得
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL; // 登録されていない数出ない場合はエラーとしてNULL
	}
	return g_pTextures[index]; // 指定IDのインターフェースを返す
}

int Texture_GetWidth(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_TextureFiles[index].width;
}

int Texture_GetHeight(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_TextureFiles[index].height;
}