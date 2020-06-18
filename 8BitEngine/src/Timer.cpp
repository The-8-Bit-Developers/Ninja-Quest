#include "Timer.h"

#include <iostream>

Timer::Timer() {}

void Timer::BeginFrame() { BeginTimer(); }
void Timer::EndFrame() { m_fDelta = EndTimer(); }

void Timer::BeginTimer()
{
	m_TimingStack.push(std::chrono::high_resolution_clock::now());
}

float Timer::EndTimer()
{
	auto start = m_TimingStack.top();
	m_TimingStack.pop();

	auto end = std::chrono::high_resolution_clock::now();
	float duration = (float) (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000.0f);
	return duration;
}

Timer::~Timer() {}