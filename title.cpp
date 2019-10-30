#include "title.h"
#include "input.h"

static Title title;

// Titleの初期化
void Title_Initialize()
{

}

// Titleの終了処理
void Title_Finalize()
{

}

// Titleの更新
void Title_Update()
{
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		SetScene(SCENE_MODE);
	}
}

// Titleの描画
void Title_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// テクスチャのセット
	pDevice->SetTexture(0, Texture_GetTexture(title.TextureIndex));

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
	if (title.bUse) // 使用中なら処理
	{
		Sprite_SetColor(title.color); // 色のセット
		// スプライト描画
		Sprite_Draw(title.TextureIndex,
			title.pos.x, title.pos.y,
			title.tx, title.ty,
			title.tw, title.th);
	}
}

Title::Title()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = SCREEN_HEIGHT * 0.5f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_TITLE;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

Title::~Title()
{

}