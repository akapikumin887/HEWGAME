//====================================================
// ���͏��� [input.cpp]
//====================================================
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

//====================================================
// �}�N����`
//====================================================
#define	NUM_KEY_MAX (256) // �L�[�̍ő吔

//====================================================
// �v���g�^�C�v�錾
//====================================================
static bool initialize(HINSTANCE hInstance);
static void finalize(void);

//====================================================
// �O���[�o���ϐ�
//====================================================
LPDIRECTINPUT8 g_pInput = NULL; // DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL; // ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX]; // �L�[�{�[�h�̓��͏�񃏁[�N
BYTE g_aKeyStateTrigger[NUM_KEY_MAX]; // �L�[�{�[�h�̃g���K�[��񃏁[�N
BYTE g_aKeyStateRelease[NUM_KEY_MAX]; // �L�[�{�[�h�̃����[�X��񃏁[�N
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL; // ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
DIMOUSESTATE g_MouseState; // �}�E�X���
DIMOUSESTATE g_MouseState_bak; // �}�E�X���(�ω����m�p)

//====================================================
// ���͏����̏�����
//====================================================
bool initialize(HINSTANCE hInstance)
{
	if(g_pInput == NULL) {

		// DirectInput�I�u�W�F�N�g�̍쐬
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) 
		{
			return false;
		}
	}
	return true;
}

//====================================================
// ���͏����̏I������
//====================================================
void finalize(void)
{
	if(g_pInput != NULL) 
	{
		// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//====================================================
// �L�[�{�[�h�̏�����
//====================================================
bool Keyboard_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if( !initialize(hInstance) ) 
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return true;
}

//====================================================
// �L�[�{�[�h�̏I������
//====================================================
void Keyboard_Finalize(void)
{
	if(g_pDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// ���͏����̏I������
	finalize();
}

//====================================================
// �L�[�{�[�h�̍X�V����
//====================================================
void Keyboard_Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		// aKeyState[DIK_SPACE] & 0x80
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐�
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// �L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//====================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//====================================================
bool Keyboard_IsPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//====================================================
bool Keyboard_IsTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//====================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//====================================================
bool Keyboard_IsRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//====================================================
// �}�E�X�̏�����
//====================================================
bool Mouse_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if (!initialize(hInstance))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �f�o�C�X�̍쐬
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)) {
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevMouse->Acquire();
	ShowCursor(false);
	return true;
}

//====================================================
// �}�E�X�̏I������
//====================================================
void Mouse_Finalize(void)
{
	if (g_pDevMouse != NULL)
	{// ���̓f�o�C�X(�}�E�X)�̊J��
		// �}�E�X�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//====================================================
// �}�E�X�̍X�V����
//====================================================
void Mouse_Update(void)
{	
	// �f�o�C�X����f�[�^���擾
	if (FAILED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState)))
	{
		g_pDevMouse->Acquire();
		g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState);
	}
}

DIMOUSESTATE* GetMouseState(void)
{
	return &g_MouseState;
}