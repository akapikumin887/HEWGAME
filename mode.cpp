#include "mode.h"
#include "input.h"
#include "debug_font.h"
#include "player.h"

static Mode mode_bg;
static Mode mode_easy;
static Mode mode_hard;
ModeIndex ModeIdx = MODE_NONE;

// Modeの初期化
void Mode_Initialize()
{
	mode_bg.TextureIndex = TEXTURE_INDEX_MODE_BG;
	mode_bg.tw = Texture_GetWidth(mode_bg.TextureIndex);
	mode_bg.th = Texture_GetHeight(mode_bg.TextureIndex);
	mode_bg.pos.x = SCREEN_WIDTH * 0.5f;
	mode_bg.pos.y = SCREEN_HEIGHT * 0.5f;

	mode_easy.TextureIndex = TEXTURE_INDEX_MODE_EASY;
	mode_easy.tw = Texture_GetWidth(mode_easy.TextureIndex);
	mode_easy.th = Texture_GetHeight(mode_easy.TextureIndex);
	mode_easy.pos.x = SCREEN_WIDTH * 0.5f;
	mode_easy.pos.y = SCREEN_HEIGHT * 0.5f - Texture_GetHeight(mode_easy.TextureIndex) * 1.5f;

	mode_hard.TextureIndex = TEXTURE_INDEX_MODE_HARD;
	mode_hard.tw = Texture_GetWidth(mode_hard.TextureIndex);
	mode_hard.th = Texture_GetHeight(mode_hard.TextureIndex);
	mode_hard.pos.x = SCREEN_WIDTH * 0.5f;
	mode_hard.pos.y = SCREEN_HEIGHT * 0.5f + Texture_GetHeight(mode_hard.TextureIndex) * 1.5f;

	Player_Initialize();
}

// Modeの終了処理
void Mode_Finalize()
{
	Player_Finalize();
}

// Modeの更新
void Mode_Update()
{
	Player *player = GetPlayer();

	Player_Update();

	// 横判定
	if ((player->pos.x + player->tw * 0.25f >= mode_easy.pos.x - mode_easy.tw * 0.5f) && 
		(player->pos.x - player->tw * 0.25f <= mode_easy.pos.x + mode_easy.tw * 0.5f))
	{
		// 縦判定（easy）
		if ((player->pos.y + player->th * 0.25f >= mode_easy.pos.y - mode_easy.th * 0.5f) &&
			(player->pos.y - player->th * 0.25f <= mode_easy.pos.y + mode_easy.th * 0.5f))
		{
			ModeIdx = MODE_EASY;
		}
		// 縦判定（hard）
		else if ((player->pos.y + player->th * 0.25f >= mode_hard.pos.y - mode_hard.th * 0.5f) &&
			(player->pos.y - player->th * 0.25f <= mode_hard.pos.y + mode_hard.th * 0.5f))
		{
			ModeIdx = MODE_HARD;
		}
		else
		{
			ModeIdx = MODE_NONE;
		}
	}
	else
	{
		ModeIdx = MODE_NONE;
	}

	if (Keyboard_IsTrigger(DIK_RETURN) && ModeIdx != MODE_NONE)
	{
		SetScene(SCENE_GAME);
	}
}

// Modeの描画
void Mode_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// テクスチャのセット
	pDevice->SetTexture(0, Texture_GetTexture(mode_bg.TextureIndex));
	pDevice->SetTexture(0, Texture_GetTexture(mode_easy.TextureIndex));
	pDevice->SetTexture(0, Texture_GetTexture(mode_hard.TextureIndex));

	// ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// スプライト描画
	if (mode_bg.bUse) // 使用中なら処理
	{
		Sprite_SetColor(mode_bg.color); // 色のセット
		// スプライト描画
		Sprite_Draw(mode_bg.TextureIndex,
			mode_bg.pos.x, mode_bg.pos.y,
			mode_bg.tx, mode_bg.ty,
			mode_bg.tw, mode_bg.th);
	}
	if (mode_easy.bUse) // 使用中なら処理
	{
		Sprite_SetColor(mode_easy.color); // 色のセット
		// スプライト描画
		Sprite_Draw(mode_easy.TextureIndex,
			mode_easy.pos.x, mode_easy.pos.y,
			mode_easy.tx, mode_easy.ty,
			mode_easy.tw, mode_easy.th);
	}
	if (mode_hard.bUse) // 使用中なら処理
	{
		Sprite_SetColor(mode_hard.color); // 色のセット
		// スプライト描画
		Sprite_Draw(mode_hard.TextureIndex,
			mode_hard.pos.x, mode_hard.pos.y,
			mode_hard.tx, mode_hard.ty,
			mode_hard.tw, mode_hard.th);
	}
	Player_Draw();
	DebugFont_Draw(2, 32, "tw: %d th: %d", mode_bg.tw, mode_bg.th);
	DebugFont_Draw(2, 62, "tw: %d th: %d", mode_easy.tw, mode_easy.th);
	DebugFont_Draw(2, 92, "tw: %d th: %d", mode_hard.tw, mode_hard.th);
	DebugFont_Draw(2, 122, "Mode: %d", ModeIdx);
}

// Mode情報の取得
ModeIndex GetMode()
{
	return ModeIdx;
}

Mode::Mode()
{
	bUse = true;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	tx = 0;
	ty = 0;
}

Mode::~Mode()
{

}