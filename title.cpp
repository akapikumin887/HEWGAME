#include "title.h"
#include "input.h"
#include "fade.h"

static Title bg;
static Title title;
static Title start;
static Title ranking;
static bool fade = false;

// Titleの初期化
void Title_Initialize()
{
	bg.Initialize(TEXTURE_INDEX_BG00);
	title.Initialize(TEXTURE_INDEX_TITLE01);
	start.Initialize(TEXTURE_INDEX_TITLE_START, D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 170.0f));
	ranking.Initialize(TEXTURE_INDEX_TITLE_RANKING, D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 400.0f));
}

// Titleの終了処理
void Title_Finalize()
{

}

// Titleの更新
void Title_Update()
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		Fade(SCENE_GAME);
		fade = true;
		//SetScene(SCENE_GAME);
	}

	title.Fade_Move_Up();
	start.Fade_Move_Down();
	ranking.Fade_Move_Down();
	//if (start.pos.y - 110.0f >= SCREEN_HEIGHT && Get_Fade_State() == FADE_STATE_NONE)
	//{
	//	
	//}
}

// Titleの描画
void Title_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

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
	Sprite_SetColor_2D(bg.color); // 色のセット
	// スプライト描画
	Sprite_Draw_2D(bg.Texture_Index,
		bg.pos.x, bg.pos.y,
		bg.tx, bg.ty,
		bg.tw, bg.th);

	// スプライト描画
	Sprite_SetColor_2D(title.color); // 色のセット
	// スプライト描画
	Sprite_Draw_2D(title.Texture_Index,
		title.pos.x, title.pos.y,
		title.tx, title.ty,
		title.tw, title.th);

	// スプライト描画
	Sprite_SetColor_2D(start.color); // 色のセット
	// スプライト描画
	Sprite_Draw_2D(start.Texture_Index,
		start.pos.x, start.pos.y,
		start.tx, start.ty,
		start.tw, start.th);

	// スプライト描画
	Sprite_SetColor_2D(ranking.color); // 色のセット
	// スプライト描画
	Sprite_Draw_2D(ranking.Texture_Index,
		ranking.pos.x, ranking.pos.y,
		ranking.tx, ranking.ty,
		ranking.tw, ranking.th);
}

Title::Title()
{
	
}

Title::~Title()
{

}

void Title::Initialize(TextureIndex idx, D3DXVECTOR2 p)
{
	Texture_Index = idx;
	tw = Texture_GetWidth(Texture_Index);
	th = Texture_GetHeight(Texture_Index);
	bUse = true;
	pos.x = p.x;
	pos.y = p.y;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	tx = 0;
	ty = 0;
}

void Title::Fade_Move_Up()
{
	if (fade == true)
	{
		pos.y -= 10.0f;
	}
}

void Title::Fade_Move_Down()
{
	if (fade == true)
	{
		pos.y += 10.0f;
	}
}