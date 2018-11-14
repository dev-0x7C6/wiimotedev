#pragma once

#include "emulation/input-device.h"

namespace io {
namespace emulation {

class EventDevice final : public InputDevice {
public:
	explicit EventDevice(std::string &&name, u32 id);

	bool pressKey(u16 key);
	bool releaseKey(u16 key);
	bool tapKey(u16 key);
	bool moveMousePointer(i32 x, i32 y);
	bool moveMouseVWheel(i32 delta);
	bool moveMouseHWheel(i32 delta);

	bool configure() override;
};
}
}
