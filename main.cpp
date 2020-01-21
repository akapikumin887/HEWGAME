#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include <windows.h>
#include <stdlib.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"
#include "xmodel.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "time.h"
#include "system_timer.h"
#include "input.h"
#include "tool_functions.h"
#include "camera.h"
#include "light.h"
#include "title.h"
#include "mode.h"
#include "game.h"
#include "plane.h"
#include "aiming.h"
#include "arrow.h"
#include "target.h"
#include "gravility.h"
#include "wind.h"
#include "ui.h"
#include "score.h"
#include "fade.h"

//====================================================
// �萔��`
//====================================================
#define CLASS_NAME     "GameWindow"       // �E�C���h�E�N���X�̖��O
#define WINDOW_CAPTION "MOUSE�ŏƏ��@LSHIFT�ō\���@Z�Ŕ��ˁ@C�Ŏ��R��ԁ@J�ŃX�s�[�h�_�E���@K�ŃX�s�[�h�A�b�v�@RSHIFT�ŃQ�[�����Z�b�g�@ENTER�ŃJ�������Z�b�g" // �E�B���h�E�̖��O
#define FPS_MEASUREMENT_TIME (1.0f)       // FPS�v������

//====================================================
// �v���g�^�C�v�錾
//====================================================
// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �Q�[���̏������֐�
// �߂�l:�������Ɏ��s�����Ƃ�false
static bool Initialize(void);
// �Q�[���̍X�V�֐�
static void Update(void);
// �Q�[���̕`��֐�
static void Draw(void);
// �Q�[���̏I������
static void Finalize(void);

//====================================================
// �O���[�o���ϐ��錾
//====================================================
static HWND g_hWnd; // �E�B���h�E�n���h��
static LPDIRECT3D9 g_pD3D = NULL; // Direct3D�C���^�[�t�F�[�X
// Direct3D�f�o�C�X
static LPDIRECT3DTEXTURE9 g_pTexture = NULL; // �e�N�X�`���C���^�[�t�F�[�X

static float g_UVScrollValue = 0.0f;

static Timer g_System_Time;
static int g_FrameCount = 0; // �t���[���J�E���^�[
static double g_StaticFrameTime = 0.0f; // �t���[���Œ�p�v������
SCENE g_Scene = SCENE_NONE;

//====================================================
//���C��
//====================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �g�p���Ȃ��ꎞ�ϐ��𖾎�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// �E�B���h�E�N���X�\���̂̐ݒ�
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc; // �E�B���h�E�v���V�[�W���̎w��
	wc.lpszClassName = CLASS_NAME; // �N���X���̐ݒ�
	wc.hInstance = hInstance; // �C���X�^���X�n���h���̎w��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // �}�E�X�J�[�\�����w��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�

	// �N���X�o�^
	RegisterClass(&wc);

	// �E�B���h�E�X�^�C��
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// ��{��`���W
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// �V����Window�̋�`���W���畝�ƍ������Z�o
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
	// ��������������A�f�X�N�g�b�v���E�B���h�E���傫���ꍇ�͍���ɕ\��
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// �E�B���h�E�̐���
	g_hWnd = CreateWindow
	(
		CLASS_NAME,     // �E�B���h�E�N���X
		WINDOW_CAPTION, // �E�B���h�E�e�L�X�g
		window_style,   // �E�B���h�E�X�^�C��
		window_x,       // �E�B���h�E���Wx
		window_y,       // �E�B���h�E���Wy
		window_width,   // �E�B���h�E�̕�
		window_height,  // �E�B���h�E�̍���
		NULL,           // �e�E�B���h�E�n���h��
		NULL,           // ���j���[�n���h��
		hInstance,      // �C���X�^���X�n���h��
		NULL            // �ǉ��̃A�v���P�[�V�����f�[�^
	);

	if (g_hWnd == NULL)
	{
		// �E�B���h�E�n���h�������炩�̗��R�Ő����o���Ȃ�����
		return -1;
	}

	// �w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
	ShowWindow(g_hWnd, nCmdShow);

	// �Q�[���̏�����(Direct3D�̏�����)
	if (!Initialize())
	{
		// �Q�[���̏������Ɏ��s����
		return -1;
	}

	// Windows�Q�[���p���C�����[�v
	MSG msg = {}; // msg.message == WM_NULL

	Input_Initialize(hInstance, g_hWnd);

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// ���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ���݂̃V�X�e�����Ԃ��擾
			double time = g_System_Time.SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / FPS_STANTARD) {
				// 1 / 60 �b�o���Ă��Ȃ����������
				Sleep(0);
			}
			else {
				// �t���[���Œ�p�̌v�����Ԃ��X�V����
				g_StaticFrameTime = time; // �Q�[���������s�������Ԃ��L�^
				// �Q�[���̍X�V
				Update();
				// �Q�[���̕`��
				Draw();
			}
		}
	}

	Finalize();

	return (int)msg.wParam;
}

// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSE���b�Z�[�W�̑��M
		}
		break;

	case WM_CLOSE:
		//ShowCursor(true);
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd); // �w��̃E�B���h�E��WM_DESTROY���b�Z�[�W�𑗂�
		}
		//ShowCursor(false);
		return 0; // DefWindowProc�֐��Ƀ��b�Z�[�W�𗬂����I�����邱�Ƃɂ���ĉ����Ȃ��������Ƃɂ���

	case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M
		return 0;
	};

	// �ʏ탁�b�Z�[�W�����͂��̊֐��ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �Q�[���̏������֐�
bool Initialize(void)
{
	//g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	srand((unsigned int)time(NULL));

	if (!Mydirect3D_Initialize(g_hWnd))
	{
		// Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s
		// MessageBox(g_hWnd, "Direct3D�C���^�[�t�F�[�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	Sprite_Initialize_2D();

	DebugFont_Initialize();

	// �e�N�X�`���̓ǂݍ���
	if (Texture_Load() > 0) {
		MessageBox(g_hWnd, "�������ǂݍ��߂Ȃ������e�N�X�`���t�@�C��������܂�", "�G���[", MB_OK);
	}

	SetScene(SCENE_TITLE);
	//SetScene(SCENE_MODE);
	//SetScene(SCENE_GAME);

	//InitSound(g_hWnd);

	//Light_Initialize();
	
	// �V�X�e���^�C�}�[�̏�����
	g_System_Time.SystemTimer_Initialize();

	// �V�X�e���^�C�}�[�̋N��
	g_System_Time.SystemTimer_Start();

	// �t���[���Œ�p�v������
	g_StaticFrameTime = g_System_Time.SystemTimer_GetTime();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // ���ʂ��J�����O
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                   // Z�o�b�t�@���g�p
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);         // default�l
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	ShowCursor(false);
	return true;
}

// �Q�[���̍X�V�֐�
void Update(void)
{
	Input_Update();

	//Light_Update();
	
	if (Get_Fade_State() == FADE_STATE_NONE)
	{
		switch (g_Scene)
		{
		case SCENE_TITLE:
			Title_Update();
			break;
		case SCENE_MODE:
			Mode_Update();
			break;
		case SCENE_GAME:
			Game_Update();
			break;
		case SCENE_RESULT:

			break;
		case SCENE_RANKING:
			//Ranking_Update();
			break;
		}
	}
	else
	{
		Fade_Update();
	}
}

// �Q�[���̕`��֐�
void Draw(void)
{
	static LPDIRECT3DDEVICE9 pD3DDevice = MyDirect3D_GetDevice();

	// ��ʂ̃N���A
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(200, 200, 0, 255), 1.0f, 0);

	// �`��o�b�`���߂̊J�n
	pD3DDevice->BeginScene();

	//Light_Draw();

	DebugFont_Draw(640, 2, "Time: %.02lf", g_System_Time.SystemTimer_GetTime());

	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Draw();
		break;
	case SCENE_MODE:
		Mode_Draw();
		break;
	case SCENE_GAME:
		Game_Draw();
		break;
	case SCENE_RESULT:

		break;
	case SCENE_RANKING:
		//Ranking_Draw();
		break;
	default:
		break;
	}
	
	Fade_Draw();

	// �`��o�b�`���߂̏I��
	pD3DDevice->EndScene();

	// �o�b�N�o�b�t�@���t���b�v�i�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��j
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// �Q�[���̏I������
void Finalize(void)
{
	// �e�N�X�`���C���^�[�t�F�[�X�̉��
	if (g_pTexture) 
	{
		g_pTexture->Release();
		g_pTexture = NULL;
		Mydirect3D_Finalize();
	}

	//UninitSound();

	//Light_Finalize();
	Sprite_Finalize_2D();
	Texture_Release();
	// �Q�[���̏I������(Direct3D�̏I������)
	Input_Finalize();
	DebugFont_Finalize();
	g_System_Time.SystemTimer_Finalize();
	Mydirect3D_Finalize();
}

void SetScene(SCENE s)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Finalize();
		break;
	case SCENE_MODE:
		Mode_Finalize();
		break;
	case SCENE_GAME:
		Game_Finalize();
		break;
	case SCENE_RESULT:

		break;
	case SCENE_RANKING:
		//Ranking_Finalize();
		break;
	default:
		break;
	}

	switch (s)
	{
	case SCENE_TITLE:
		Title_Initialize();
		break;
	case SCENE_MODE:
		Mode_Initialize();
		break;
	case SCENE_GAME:
		Game_Initialize();
		break;
	case SCENE_RESULT:

		break;
	case SCENE_RANKING:
		//Ranking_Initialize();
		break;
	default:
		break;
	}
	g_Scene = s;
}

SCENE GetScene()
{
	return g_Scene;
}