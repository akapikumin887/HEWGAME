#include "fade.h"

#define FADECNT 30
FADE_STATE g_FadeState;
float g_FadeAlpha = 0.0f;
SCENE g_NextScene;

// Fadeの初期化
void Fade_Initialize()
{

}

// Fadeの終了処理
void Fade_Finalize()
{

}

// Fadeの更新
void Fade_Update()
{
	switch (g_FadeState)
	{
	case FADE_STATE_OUT:
		//フェードアウト（アルファ+）
		g_FadeAlpha += 255.0f / FADECNT;
		if (g_FadeAlpha > 255.0f)
		{
			g_FadeAlpha = 255.0f;
			SetScene(g_NextScene);
			g_FadeState = FADE_STATE_IN;
		}
		break;
	case FADE_STATE_IN:
		g_FadeAlpha -= 255.0f / FADECNT;
		if (g_FadeAlpha < 0.0f)
		{
			g_FadeAlpha = 0.0f;
			g_FadeState = FADE_STATE_NONE;
		}
		break;
	}
}

// Fadeの描画
void Fade_Draw()
{
	if (g_FadeState == FADE_STATE_IN || g_FadeState == FADE_STATE_OUT)
	{
		LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

		// テクスチャのセット
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MAX));

		// スプライト描画
		Sprite_SetColor_2D(D3DCOLOR_RGBA(0, 0, 0, (int)g_FadeAlpha)); // 色のセット

		Sprite_Draw_2D(TEXTURE_INDEX_MAX,
			SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,
			0, 0,
			SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

void Fade(SCENE s)
{
	g_FadeState = FADE_STATE_OUT;
	g_FadeAlpha = 0.0f;
	g_NextScene = s;
}

FADE_STATE Get_Fade_State()
{
	return g_FadeState;
}