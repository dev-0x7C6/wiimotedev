#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <interfaces/igamepad.h>

namespace io {
namespace factory {

class GamepadFactory {
public:
	explicit GamepadFactory() = delete;
	virtual ~GamepadFactory() = delete;

	static std::unique_ptr<io::emulation::gamepad::IGamepad> create(const io::emulation::gamepad::IGamepad::Type type, const std::string &name, const uint32_t id);
	static bool configure(std::unique_ptr<io::emulation::gamepad::IGamepad> &interface);
};
}
}
