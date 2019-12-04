#include <Windows.h>
#include "system_timer.h"

static Timer *timer;

//====================================================
// �֐���`
//====================================================
// �V�X�e���^�C�}�[�̏�����
void SystemTimer_Initialize(void)
{
	timer = new Timer;
}

// �V�X�e���^�C�}�[�̏I������
void SystemTimer_Finalize(void)
{
	delete timer;
}

// Time�̏������i�R���X�g���N�^�j
Timer::Timer()
{
	bTimerStopped = true;
	TicksPerSec = 0;
	StopTime = 0;
	LastElapsedTime = 0;
	BaseTime = 0;

	// ������\�p�t�H�[�}���X�J�E���^���g���̎擾
	LARGE_INTEGER ticksPerSec = { 0 };
	QueryPerformanceFrequency(&ticksPerSec);
	TicksPerSec = ticksPerSec.QuadPart;
}

void Timer::SystemTimer_Reset(void)
{
	LARGE_INTEGER time = GetAdjustedCurrentTime();

	BaseTime = LastElapsedTime = time.QuadPart;
	StopTime = 0;
	bTimerStopped = false;
}

void Timer::SystemTimer_Start(void)
{
	// ���݂̎��Ԃ��擾
	LARGE_INTEGER time = { 0 };
	QueryPerformanceCounter(&time);

	// ���܂Ōv�����X�g�b�v���Ă�����
	if (bTimerStopped) {
		// �~�܂��Ă������Ԃ����������Ċ�{���Ԃ��X�V
		BaseTime += time.QuadPart - StopTime;
	}

	StopTime = 0;
	LastElapsedTime = time.QuadPart;
	bTimerStopped = false;
}

void Timer::SystemTimer_Stop(void)
{
	if (bTimerStopped) return;

	LARGE_INTEGER time = { 0 };
	QueryPerformanceCounter(&time);

	LastElapsedTime = StopTime = time.QuadPart; // ��~���Ԃ��L�^
	bTimerStopped = true;
}

void Timer::SystemTimer_Advance(void)
{
	StopTime += TicksPerSec / 10;
}

double Timer::SystemTimer_GetTime(void)
{
	LARGE_INTEGER time = GetAdjustedCurrentTime();

	return (double)(time.QuadPart - BaseTime) / (double)TicksPerSec;
}

double Timer::SystemTimer_GetAbsoluteTime(void)
{
	LARGE_INTEGER time = { 0 };
	QueryPerformanceCounter(&time);

	return time.QuadPart / (double)TicksPerSec;
}

float Timer::SystemTimer_GetElapsedTime(void)
{
	LARGE_INTEGER time = GetAdjustedCurrentTime();

	double elapsed_time = (float)((double)(time.QuadPart - LastElapsedTime) / (double)TicksPerSec);
	LastElapsedTime = time.QuadPart;

	// �^�C�}�[�����m�ł��邱�Ƃ�ۏ؂��邽�߂ɁA�X�V���Ԃ��O�ɃN�����v����B
	// elapsed_time�́A�v���Z�b�T���ߓd���[�h�ɓ��邩�A���炩�̌`�ŕʂ̃v���Z�b�T�ɃV���b�t�������ƁA���͈̔͊O�ɂȂ�\��������B
	// ����āA���C���X���b�h��SetThreadAffinityMask���Ăяo���āA�ʂ̃v���Z�b�T�ɃV���b�t������Ȃ��悤�ɂ���K�v������B
	// ���̃��[�J�[�X���b�h��SetThreadAffinityMask���Ăяo���ׂ��ł͂Ȃ��A���C���X���b�h������W���ꂽ�^�C�}�[�f�[�^�̋��L�R�s�[���g�p���邱�ƁB
	if (elapsed_time < 0.0f) {
		elapsed_time = 0.0f;
	}

	return (float)elapsed_time;
}

bool Timer::SystemTimer_IsStoped(void)
{
	return bTimerStopped;
}

// ���݂̃X���b�h��1�̃v���Z�b�T�i���݂̃X���b�h�j�ɐ���
void LimitThreadAffinityToCurrentProc(void)
{
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;

    if( GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask) != 0 && dwProcessAffinityMask ) {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if( INVALID_HANDLE_VALUE != hCurrentThread ) {
            SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
            CloseHandle(hCurrentThread);
        }
    }
    CloseHandle(hCurrentProcess);
}

LARGE_INTEGER Timer::GetAdjustedCurrentTime(void)
{
	LARGE_INTEGER time;
	if (StopTime != 0) {
		time.QuadPart = StopTime;
	}
	else {
		QueryPerformanceCounter(&time);
	}
	return time;
}

// Time���̎擾
Timer* Get_Time()
{
	return timer;
}