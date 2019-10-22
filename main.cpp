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
#include <time.h>
#include "common.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "sprite.h"
#include "texture.h"
#include "system_timer.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "player.h"

//====================================================
// �萔��`
//====================================================
#define CLASS_NAME     "GameWindow"       // �E�C���h�E�N���X�̖��O
#define WINDOW_CAPTION "STG  Key��GameScene CKey��TitleScene  ZKey�Œe���ˁ@���L�[�ňړ�" // �E�B���h�E�̖��O
#define FPS_MEASUREMENT_TIME (1.0f)       // FPS�v������
#define FPS_STANTARD 60.0f

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

	Keyboard_Initialize(hInstance, g_hWnd);
	Mouse_Initialize(hInstance, g_hWnd);

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// ���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ���݂̃V�X�e�����Ԃ��擾
			double time = SystemTimer_GetTime();

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
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd); // �w��̃E�B���h�E��WM_DESTROY���b�Z�[�W�𑗂�
		}
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

	Sprite_Initialize();

	DebugFont_Initialize();

	Texture_Load();

	SetScene(SCENE_GAME);

	InitSound(g_hWnd);
	
	// �V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();

	// �V�X�e���^�C�}�[�̋N��
	SystemTimer_Start();

	// �t���[���Œ�p�v������
	g_StaticFrameTime = SystemTimer_GetTime();

	return true;
}

// �Q�[���̍X�V�֐�
void Update(void)
{
	Keyboard_Update();
	Mouse_Update();

	switch (g_Scene)
	{
	case SCENE_TITLE:

		break;
	case SCENE_GAME:
		Game_Update();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}
}

// �Q�[���̕`��֐�
void Draw(void)
{
	static LPDIRECT3DDEVICE9 pD3DDevice = Mydirect3D_GetDevice();

	// ��ʂ̃N���A
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(80, 80, 80, 255), 1.0f, 0);

	// �`��o�b�`���߂̊J�n
	pD3DDevice->BeginScene();

	pD3DDevice->SetFVF(FVF_VERTEX2D);

	switch (g_Scene)
	{
	case SCENE_TITLE:

		break;
	case SCENE_GAME:
		Game_Draw();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}

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

	UninitSound();

	Mydirect3D_Finalize();
	Sprite_Finalize();
	Texture_Release();
	// �Q�[���̏I������(Direct3D�̏I������)
	Mouse_Finalize();
	Keyboard_Finalize();
	DebugFont_Finalize();
}

void SetScene(SCENE s)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:

		break;
	case SCENE_GAME:
		Game_Finalize();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}

	switch (s)
	{
	case SCENE_TITLE:

		break;
	case SCENE_GAME:
		Game_Initialize();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}
	g_Scene = s;
}