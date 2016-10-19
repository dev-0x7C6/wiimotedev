#pragma once

#include "interfaces/iwiimotedev-event.h"

namespace io {
namespace interface {

class IMouse : public interface::IWiimotedevEvent {
public:
	explicit IMouse() = default;
	virtual ~IMouse() = default;

protected:
	virtual void connected(common::enums::Device device, uint id) override;
	virtual void disconnected(common::enums::Device device, uint id) override;
	virtual void found(common::enums::Device, uint id) override;
	virtual void lost(common::enums::Device, uint id) override;
	virtual void virtualCursorDataChanged(uint id, double x, double y, double l, double a) override;
	virtual void buttonDataChanged(common::enums::Device device, uint id, qulonglong mask) override;
	virtual void stickDataChanged(common::enums::Device device, uint id, int lx, int ly, int rx, int ry) override;
	virtual void accelerometerDataChanged(common::enums::Device device, uint id, int x, int y, int z) override;
	virtual void gyroscopeDataChanged(uint id, int x, int y, int z) override;
	virtual void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override;
	virtual void pressureDataChanged(uint id, int tl, int tr, int bl, int br) override;
};
}
}
