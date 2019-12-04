#include <Windows.h>
#include "system_timer.h"

static Timer *timer;

//====================================================
// 関数定義
//====================================================
// システムタイマーの初期化
void SystemTimer_Initialize(void)
{
	timer = new Timer;
}

// システムタイマーの終了処理
void SystemTimer_Finalize(void)
{
	delete timer;
}

// Timeの初期化（コンストラクタ）
Timer::Timer()
{
	bTimerStopped = true;
	TicksPerSec = 0;
	StopTime = 0;
	LastElapsedTime = 0;
	BaseTime = 0;

	// 高分解能パフォーマンスカウンタ周波数の取得
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
	// 現在の時間を取得
	LARGE_INTEGER time = { 0 };
	QueryPerformanceCounter(&time);

	// 今まで計測がストップしていたら
	if (bTimerStopped) {
		// 止まっていた時間を差し引いて基本時間を更新
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

	LastElapsedTime = StopTime = time.QuadPart; // 停止時間を記録
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

	// タイマーが正確であることを保証するために、更新時間を０にクランプする。
	// elapsed_timeは、プロセッサが節電モードに入るか、何らかの形で別のプロセッサにシャッフルされると、この範囲外になる可能性がある。
	// よって、メインスレッドはSetThreadAffinityMaskを呼び出して、別のプロセッサにシャッフルされないようにする必要がある。
	// 他のワーカースレッドはSetThreadAffinityMaskを呼び出すべきではなく、メインスレッドから収集されたタイマーデータの共有コピーを使用すること。
	if (elapsed_time < 0.0f) {
		elapsed_time = 0.0f;
	}

	return (float)elapsed_time;
}

bool Timer::SystemTimer_IsStoped(void)
{
	return bTimerStopped;
}

// 現在のスレッドを1つのプロセッサ（現在のスレッド）に制限
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

// Time情報の取得
Timer* Get_Time()
{
	return timer;
}