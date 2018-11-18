#include <unistd.h>

#include "tick-aligned-loop.h"

TickAlignedLoop::TickAlignedLoop()
		: m_runtime(0)
		, m_alltime(0)
		, m_loop(0) {
	m_elapsed.start();
}

u64 TickAlignedLoop::wait(u64 hz) {
	m_runtime = m_elapsed.elapsed();
	m_alltime += m_runtime;
	m_elapsed.restart();
	double delay = (1000000 / double(hz)) - m_runtime;

	if (delay < 0)
		delay = 0.0;

	usleep(delay);
	m_alltime += delay;

	if (m_alltime >= 1000000) {
		m_elapsed.restart();
		m_alltime = 0;
		m_loop++;
	}

	return m_loop;
}

u64 TickAlignedLoop::loopCount() const {
	return m_loop;
}
