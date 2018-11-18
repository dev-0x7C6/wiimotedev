#pragma once

#include <io/emulation/event-device.h>
#include "common/tick-aligned-loop.h"
#include "container/infrared-config.h"
#include "container/infrared-cursor.h"
#include "include/wiimotedev/wiimotedev"

#include <QTimer>
#include <QTime>
#include <QThread>
#include <QMutex>

#include <atomic>

namespace io {

class EIOInfraredMouse : public QThread {
public:
	enum Axis {
		XAxis = 0,
		YAxis = 1
	};

	enum Mode {
		Absolute,
		Relative
	};

	explicit EIOInfraredMouse(io::emulation::EventDevice &device, QObject *parent = nullptr);
	virtual ~EIOInfraredMouse();

	void dbusVirtualCursorPosition(u32, double, double, double, double);
	void dbusVirtualCursorLost(u32);

	u32 id() const;
	void setId(const u32 id);

	InfraredConfigContainer &config() {
		return m_config;
	}

	void interrupt();

protected:
	void run();

	void processAbsoluteMouse();
	void processRelativeMouse();

	void setCursor(container::InfraredCursor &&cursor);

private:
	double m_position[2];
	double m_delta[2];
	double m_size[2];
	double m_acc[2];
	i32 m_timeout;

	InfraredConfigContainer m_config;

private:
	io::emulation::EventDevice &m_device;
	container::InfraredCursor m_cursor;

	std::atomic<u32> m_id{1};
	std::atomic<u32> m_mode{Relative};
	std::atomic<bool> m_interrupted{false};
	QMutex m_mutex;
	TickAlignedLoop m_tick;
};

}
