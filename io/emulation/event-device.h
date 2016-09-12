#pragma once

#include "emulation/input-device.h"

namespace io {
namespace emulation {

class EventDevice final : public InputDevice {
public:
	explicit EventDevice(const std::string &name, const uint32_t id);

	bool pressKey(const uint16_t key);
	bool releaseKey(const uint16_t key);
	bool tapKey(const uint16_t key);
	bool moveMousePointer(const int32_t x, const int32_t y);
	bool moveMouseVWheel(const int32_t delta);
	bool moveMouseHWheel(const int32_t delta);

	virtual bool configure() override;
};
}
}
