#include "cube.h"
#include "debug_font.h"
#include "aiming.h"
#include "target.h"
#include "wall.h"

static Cube cube[CUBE_MAX];
static Aiming *aiming;
int Cube::cnt = 0;

// Cubeの初期化
void Cube_Initialize()
{
	for (int i = 0; i < CUBE_MAX; i++)
	{
		cube[i].bUse = false;
	}
}

// Cubeの終了処理
void Cube_Finalize()
{
	for (int i = 0; i < CUBE_MAX; i++)
	{
		cube[i].bUse = false;
		cube[i].bShotted = false;
		cube[i].nShotted = false;
		cube[i].bHit = false;
		cube[i].pos.x = 0.0f;
		cube[i].pos.y = 0.0f;
		cube[i].pos.z = 0.0f;
		cube[i].targetLen = 0.0f;
	}
}

// Cubeの更新
void Cube_Update()
{
	Cube::cnt = 0;
	for (int i = 0; i < CUBE_MAX; i++)
	{	
		// 使用中
		if (cube[i].bUse)
		{
			// 時間制限を超えていない
			if (!cube[i].nShotted)
			{
				// 発射されていない
				if (!cube[i].bShotted)
				{
					cube[i].Cube_Aiming_Direction();
				}
				// 発射されたら
				else
				{
					// 命中していない
					if (!cube[i].bHit)
					{
						// 速度更新
						cube[i].Cube_Move_Direction(D3DXVECTOR3(10.0f,0.0f,0.0f));
						cube[i].move = cube[i].direction * MOVE_SPEED;

						// Cubeの位置更新
						cube[i].posOld = cube[i].pos;
						cube[i].pos += cube[i].move;

						// Cubeの先頭位置の更新
						cube[i].posHead.y = cube[i].pos.y - cube[i].scl.z / 2 * sinf(D3DXToRadian(cube[i].rot.x));
						cube[i].posHead.x = cube[i].pos.x + cube[i].scl.z / 2 * sinf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));
						cube[i].posHead.z = cube[i].pos.z + cube[i].scl.z / 2 * cosf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));

						// 先頭位置が的を超えた場合
						if (cube[i].posHead.z > cube[i].posAiming.z)
						{
							// 的での刺し位置の計算
							float lenRatio = (cube[i].posAiming.z - cube[i].pos.z) / (cube[i].scl.z * cosf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x)));
							cube[i].posHit.y = cube[i].pos.y - cube[i].scl.z * lenRatio * sinf(D3DXToRadian(cube[i].rot.x));
							cube[i].posHit.x = cube[i].pos.x + cube[i].scl.z * lenRatio * sinf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));
							cube[i].posHit.z = cube[i].pos.z + cube[i].scl.z * lenRatio * cosf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));
							cube[i].bHit = true;
							cube[i].pos -= (cube[i].posHead - cube[i].posHit);
							// 当たり判定
							Target *target = Get_Target();
							cube[i].targetLen = cube[i].Get_Length(cube[i].posHit - target->pos);
						}
					}
				}
			}
		}
		// 未使用の矢のカウント
		else
		{
			Cube::cnt++;
		}
	}
}

// Cubeの描画
void Cube_Draw()
{
	for (int i = 0; i < CUBE_MAX; i++)
	{
		// 使用中かつ時間制限を超えていない
		if (cube[i].bUse && !cube[i].nShotted)
		{
			cube[i].Draw_Cube();
		}
	}
	DebugFont_Draw(2, 122, "矢の残数: %d", Cube::cnt);
	for (int i = 0; i < CUBE_MAX; i++)
	{
		DebugFont_Draw(2, 152 + i * 30, "Cube  x: %.2lf  y: %.2lf  z: %.10lf", cube[i].pos.x, cube[i].pos.y, cube[i].pos.z);
		DebugFont_Draw(2, 302 + i * 30, "Len: %.10lf", cube[i].targetLen);
	}
	
}

// Cubeの追加
void Add_Cube()
{
	for (int i = 0; i < CUBE_MAX; i++)
	{
		if (!cube[i].bUse)
		{
			cube[i].Set_Cube(TEXTURE_INDEX_KIZUNA, D3DXVECTOR3(CUBE_X, CUBE_Y, -CUBE_Z * CUBE_MAG_Z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.05f, 0.05f, 5.0f));
			break;
		}
	}
}

// Cube情報の取得
Cube* Get_Cube()
{
	return cube;
}

// Cubeの頂点情報取得（コンストラクタ）
Cube::Cube()
{	
	cubev = new VERTEX_3D;
}

// Cubeの頂点情報のリリース（デストラクタ）
Cube::~Cube()
{
	delete cubev;
}

// Cubeの描画
void Cube::Draw_Cube()
{
	cubev->Sprite_Draw_Cube(texture_index, pos, rot, scl, revolution, revRadius, revSpd);
}

// Cubeのセット
void Cube::Set_Cube(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	bUse = true;
	bShotted = false;
	nShotted = false;
	bHit = false;
	texture_index = textureindex;
	pos = posOld = p;
	rot = r;
	scl = s;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
	targetLen = 0.0f;
}

// Cubeの向きの計算
void Cube::Cube_Aiming_Direction()
{
	aiming = Get_Aiming(); // Aiming情報の取得
	direction = aiming->pos - pos; // Cubeの発射時の向きの計算
	posAiming = aiming->pos; // 発射時のAimingの位置情報の保存
	D3DXVec3Normalize(&direction, &direction); // Cubeの向きの正規化
	rot = D3DXVECTOR3(-D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCubeの回転
}

// Cubeの移動時の向き
void Cube::Cube_Move_Direction(D3DXVECTOR3 wind,float g)
{
	// 風による影響
	static int frame = 0;
	wind /= WIND_FORCE_MAG;
	if (frame != 0)
	{
		direction += D3DXVECTOR3(wind.x, g, 0.0f);
	}
	else
	{
		frame++;
	}
	rot = D3DXVECTOR3(-D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCubeの回転
}

// Cubeの回転の補正
float Cube::Rotation_Correction(float r)
{
	return cubev->Rotation_Correction(r);
}

// 長さの計算
float Cube::Get_Length(D3DXVECTOR3 l)
{
	return sqrtf(powf(l.x, 2.0f) + powf(l.y, 2.0f) + powf(l.z, 2.0f));
}