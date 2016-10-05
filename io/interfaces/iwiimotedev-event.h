#pragma once

#include <cstdint>
#include <qnamespace.h>
#include "linux/usr/include/wiimotedev/wiimotedev"

namespace io {

namespace enums {
enum class Source {
	BalanceBoard,
	Classic,
	Nunchuk,
	ProController,
	Wiimote,
};
}

namespace interface {

class IWiimotedevEvent {
public:
	explicit IWiimotedevEvent() = default;
	virtual ~IWiimotedevEvent() = default;

	virtual void connected(enums::Source, uint id) = 0;
	virtual void disconnected(enums::Source, uint id) = 0;
	virtual void buttonDataChanged(enums::Source, uint id, qulonglong mask) = 0;
	virtual void stickDataChanged(enums::Source, uint id, int lx, int ly, int rx, int ry) = 0;
	virtual void accelerometerDataChanged(enums::Source, uint id, int x, int y, int z) = 0;
	virtual void gyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ) = 0;
	virtual void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;
};
}
}
