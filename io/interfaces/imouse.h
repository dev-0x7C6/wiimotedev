#pragma once

#include "interfaces/iwiimotedev-event.h"

namespace io {
namespace interface {

class IMouse : public interface::IWiimotedevEvent {
protected:
	void connected(common::enums::device device, uint id) override{};
	void disconnected(common::enums::device device, uint id) override{};
	void found(common::enums::device, uint id) override{};
	void lost(common::enums::device, uint id) override{};
	void virtualCursorDataChanged(uint id, double x, double y, double l, double a) override{};
	void buttonDataChanged(common::enums::device device, uint id, qulonglong mask) override{};
	void stickDataChanged(common::enums::device device, uint id, int lx, int ly, int rx, int ry) override{};
	void accelerometerDataChanged(common::enums::device device, uint id, int x, int y, int z) override{};
	void gyroscopeDataChanged(uint id, int x, int y, int z) override{};
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override{};
	void pressureDataChanged(uint id, int tl, int tr, int bl, int br) override{};
};
}
}
