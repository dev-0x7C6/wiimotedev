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

	virtual void connected(common::enums::device device, uint id) = 0;
	virtual void disconnected(common::enums::device device, uint id) = 0;
	virtual void found(common::enums::device, uint id) = 0;
	virtual void lost(common::enums::device, uint id) = 0;
	virtual void virtualCursorDataChanged(uint id, double x, double y, double l, double a) = 0;
	virtual void buttonDataChanged(common::enums::device device, uint id, qulonglong mask) = 0;
	virtual void stickDataChanged(common::enums::device device, uint id, int lx, int ly, int rx, int ry) = 0;
	virtual void accelerometerDataChanged(common::enums::device device, uint id, int x, int y, int z) = 0;
	virtual void gyroscopeDataChanged(uint id, int x, int y, int z) = 0;
	virtual void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;
	virtual void pressureDataChanged(uint id, int tl, int tr, int bl, int br) = 0;
};
}
}
