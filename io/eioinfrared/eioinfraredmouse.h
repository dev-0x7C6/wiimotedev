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

	void dbusVirtualCursorPosition(uint32_t, double, double, double, double);
	void dbusVirtualCursorLost(uint32_t);

	uint32_t id() const;
	void setId(const uint32_t id);

	InfraredConfigContainer &config() {
		return m_config;
	}

	void interrupt();

protected:
	void run();

	void processAbsoluteMouse();
	void processRelativeMouse();

	void setCursor(InfraredCursor &&cursor);

private:
	double m_position[2];
	double m_delta[2];
	double m_size[2];
	double m_acc[2];
	int32_t m_timeout;

	InfraredConfigContainer m_config;

private:
	io::emulation::EventDevice &m_device;
	InfraredCursor m_cursor;

	std::atomic<uint32_t> m_id;
	std::atomic<uint32_t> m_mode;
	std::atomic<bool> m_interrupted;
	QMutex m_mutex;
	TickAlignedLoop m_tick;
};
