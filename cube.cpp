#include "cube.h"
#include "input.h"
#include "debug_font.h"
#include "camera.h"
#include "aiming.h"
#include "target.h"
#include "wall.h"
#include "gravility.h"
#include "wind.h"
#include "score.h"

static Cube cube[CUBE_MAX];
static Camera *camera;
static Aiming *aiming;
static Wind *wind;
static Gravility *gravility;
static Target *target;
int Cube::cnt = 0;
float speed;

// Cubeの初期化
void Cube_Initialize()
{
	for (int i = 0; i < CUBE_MAX; i++)
	{
		cube[i].bUse = false;
	}
	camera = Get_Camera();
	gravility = Get_Gravility();
	wind = Get_Wind();
	speed = MOVE_SPEED;
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

	if (Keyboard_IsTrigger(DIK_J))
	{
		speed -= 0.1f * MOVE_SPEED;
	}

	if (Keyboard_IsTrigger(DIK_K))
	{
		speed += 0.1f * MOVE_SPEED;
	}
	
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
						camera->bZoom_Ready = true; // 飛行中に、ズーム後退の準備
						// 移動量の更新
						cube[i].move = cube[i].direction * speed + wind->speed + gravility->g;
						//cube[i].move = cube[i].direction * MOVE_SPEED;

						// 向き更新のため、元の位置情報を記録
						cube[i].posOld = cube[i].pos;

						// Cubeの位置更新
						cube[i].pos += cube[i].move;

						// 向き更新
						cube[i].Cube_Move_Direction();

						// Cubeの先頭位置の更新
						cube[i].posHead.y = cube[i].pos.y - cube[i].scl.z / 2 * sinf(D3DXToRadian(cube[i].rot.x));
						cube[i].posHead.x = cube[i].pos.x + cube[i].scl.z / 2 * sinf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));
						cube[i].posHead.z = cube[i].pos.z + cube[i].scl.z / 2 * cosf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));

						// 先頭位置が的を超えた場合
						if (cube[i].posHead.z > cube[i].posAiming.z)
						{
							camera->bZoom_Back = true;
							camera->bZoom_Ready = false;
							// 的での刺し位置の計算
							float lenRatio = (cube[i].posAiming.z - cube[i].pos.z) / (cube[i].scl.z * cosf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x)));
							cube[i].posHit.y = cube[i].pos.y - cube[i].scl.z * lenRatio * sinf(D3DXToRadian(cube[i].rot.x));
							cube[i].posHit.x = cube[i].pos.x + cube[i].scl.z * lenRatio * sinf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));
							cube[i].posHit.z = cube[i].pos.z + cube[i].scl.z * lenRatio * cosf(D3DXToRadian(cube[i].rot.y)) * cosf(D3DXToRadian(cube[i].rot.x));
							cube[i].bHit = true;
							cube[i].pos -= (cube[i].posHead - cube[i].posHit);
							// 当たり判定
							target = Get_Target();
							cube[i].targetLen = cube[i].Get_Length(cube[i].posHit - target->pos);
							cube[i].Score();
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
	DebugFont_Draw(640, 92, "speed: %.02lf", speed);
	//Sprite_Draw_2D(TEXTURE_INDEX_KIZUNA, 128.0f, 128.0f, 0, 0, 256, 256);
}

// Cubeの追加
void Add_Cube()
{
	for (int i = 0; i < CUBE_MAX; i++)
	{
		if (!cube[i].bUse)
		{
			cube[i].Set_Cube(TEXTURE_INDEX_KIZUNA, D3DXVECTOR3(CUBE_X, CUBE_Y, -CUBE_Z * CUBE_MAG_Z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 6.0f));
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
void Cube::Cube_Move_Direction()
{
	direction = pos - posOld;
	D3DXVec3Normalize(&direction, &direction); // Cubeの向きの正規化
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

// Score判定
void Cube::Score()
{
	if (targetLen <= target->circle[0])
	{
		Add_Score(100);
	}
	else if (targetLen > target->circle[0] && targetLen <= target->circle[1])
	{
		Add_Score(75);
	}
	else if (targetLen > target->circle[1] && targetLen <= target->circle[2])
	{
		Add_Score(50);
	}
	else if (targetLen > target->circle[2] && targetLen <= target->circle[3])
	{
		Add_Score(25);
	}
	else if (targetLen > target->circle[3] && targetLen <= target->circle[4])
	{
		Add_Score(20);
	}
	else if (targetLen > target->circle[4] && targetLen <= target->circle[5])
	{
		Add_Score(15);
	}
	else if (targetLen > target->circle[5] && targetLen <= target->circle[6])
	{
		Add_Score(10);
	}
	else if (targetLen > target->circle[6] && targetLen <= target->circle[7])
	{
		Add_Score(5);
	}
	else if (targetLen > target->circle[7] && targetLen <= target->circle[8])
	{
		Add_Score(3);
	}
	else if (targetLen > target->circle[8] && targetLen <= target->circle[9])
	{
		Add_Score(2);
	}
	else if (targetLen > target->circle[9] && targetLen <= target->circle[10])
	{
		Add_Score(1);
	}
}