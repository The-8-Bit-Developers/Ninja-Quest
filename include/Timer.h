#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>
#include <stack>

/*
	The Time class keeps track of things such as delta-time,
	and provided profiling tools too.
*/
class Timer
{

public:

	Timer();
	~Timer();

	void BeginFrame();
	void EndFrame();

	void BeginTimer();
	float EndTimer();

	float m_fDelta;

private:
	std::stack<std::chrono::time_point<std::chrono::high_resolution_clock>> m_TimingStack;
};

#endif