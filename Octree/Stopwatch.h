#ifndef _WIN32
#include <sys/time.h>

double stopwatch()
{
	struct timeval time;
	gettimeofday(&time, 0 );
	return 1.0 * time.tv_sec + time.tv_usec / (double)1e6;
}

#else

#include <windows.h>

inline double stopwatch() 
{
	unsigned long long ticks;
	unsigned long long ticks_per_sec;
	QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER *>(&ticks_per_sec));
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&ticks));
	return static_cast<float>(ticks) / static_cast<float>(ticks_per_sec);
}

#endif
