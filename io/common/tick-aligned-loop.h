#pragma once

#include <QElapsedTimer>

#include <externals/common/types.hpp>

class QElapsedTimer;

class TickAlignedLoop {
public:
	TickAlignedLoop();

	u64 wait(u64 hz = 100);
	u64 loopCount() const;

private:
	QElapsedTimer m_elapsed;
	u64 m_runtime;
	u64 m_alltime;
	u64 m_loop;
};
