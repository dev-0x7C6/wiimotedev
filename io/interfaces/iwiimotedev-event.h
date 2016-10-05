#pragma once

#include <cstdint>
#include <qnamespace.h>
#include "include/wiimotedev/wiimotedev"
#include "common/enums/device.h"

namespace io {
namespace interface {

class IWiimotedevEvent {
public:
	explicit IWiimotedevEvent() = default;
	virtual ~IWiimotedevEvent() = default;

	virtual void connected(common::enums::Device, uint id) = 0;
	virtual void disconnected(common::enums::Device, uint id) = 0;
	virtual void buttonDataChanged(common::enums::Device, uint id, qulonglong mask) = 0;
	virtual void stickDataChanged(common::enums::Device, uint id, int lx, int ly, int rx, int ry) = 0;
	virtual void accelerometerDataChanged(common::enums::Device, uint id, int x, int y, int z) = 0;
	virtual void gyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ) = 0;
	virtual void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;
};
}
}
