#include "player.h"
#include "input.h"
#include "debug_font.h"
#include "sound.h"
#include "arrow.h"
#include "target.h"

<<<<<<< HEAD
static Player player;
static Target *target = GetTarget();
=======
Player player;
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83

// Playerの初期化
void Player_Initialize()
{

}

// Playerの終了処理
void Player_Finalize()
{
	
}

// Playerの更新
void Player_Update()
{
	// マウス情報の取得
	DIMOUSESTATE *MouseState = GetMouseState();
	player.pos.x += MouseState->lX;
	player.pos.y += MouseState->lY;

	//画面外処理
	if ((player.pos.x - (float)player.tw / 2) <= 0.0f)
	{
		player.pos.x = (float)player.tw / 2;
	}
	if ((player.pos.x + (float)player.tw / 2) >= SCREEN_WIDTH)
	{
		player.pos.x = (float)(SCREEN_WIDTH - player.tw / 2);
	}
<<<<<<< HEAD
	if ((player.pos.y - (float)player.th / 2) <= target->th * 4.0f)
	{
		player.pos.y = (float)player.th / 2 + target->th * 4.0f;
=======
	if ((player.pos.y - (float)player.th / 2) <= 0.0f)
	{
		player.pos.y = (float)player.th / 2;
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
	}
	if ((player.pos.y + (float)player.th / 2) >= SCREEN_HEIGHT)
	{
		player.pos.y = (float)(SCREEN_HEIGHT - player.th / 2);
	}

	// 構え（矢の生成）
	if (Keyboard_IsTrigger(DIK_SPACE) && !player.prepare)
	{
		Create_Arrow();
		player.prepare = true;
	}

	// 構え状態中
	if (player.prepare)
	{
		// 弓を引く（チャージ）
		if (Keyboard_IsPress(DIK_Z))
		{	// SPACEを押している間チャージする
			if (player.charge_span < CHARGE_SPAN)
			{
				player.charge_span++;
			}
		}
		else
		{
			if (player.charge_span > 0)
			{
				player.charge_span--;
			}
		}

		// 発射
		if (Keyboard_IsTrigger(DIK_X) && player.charge_span > 0)
		{
			Arrow *arrow = GetArrow();
			for (int i = 0; i < ARROW_MAX; i++)
			{
				if (arrow[i].bUse && !arrow[i].beShotted)
				{
					arrow[i].beShotted = true;
					arrow[i].charge = player.charge_span;
					PlaySound(SOUND_LABEL_SE_SHOT00);
				}
			}
			player.charge_span = 0;
			player.prepare = false;
		}
	}
}

// Playerの描画
void Player_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

<<<<<<< HEAD
	// テクスチャのセット
=======
	//テクスチャのセット
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
	pDevice->SetTexture(0, Texture_GetTexture(player.TextureIndex));

	// ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

<<<<<<< HEAD
	// スプライト描画
	if (player.bUse) // 使用中なら処理
	{
		Sprite_SetColor(player.color); // 色のセット
		// スプライト描画
=======
	//スプライト描画
	if (player.bUse)//使用中なら処理
	{
		Sprite_SetColor(player.color);//色のセット
		//スプライト描画
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
		Sprite_Draw(player.TextureIndex,
			player.pos.x, player.pos.y,
			player.tx, player.ty,
			player.tw, player.th);
		DebugFont_Draw(2, 2, "x: %.2lf y: %.2lf charge: %d", player.pos.x, player.pos.y, player.charge_span);
	}
}

// プレイヤー情報の取得
Player* GetPlayer()
{
	return &player;
}

Player::Player()
{
	bUse = true; // 構造体使用中
	isShot = false;
	prepare = false;
	pos.x = SCREEN_WIDTH * 0.5f; 
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_PLAYER;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
	power = 0;
	charge_span = 0;
}

Player::~Player()
{
	bUse = false;
}