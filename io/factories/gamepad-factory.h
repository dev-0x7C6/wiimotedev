#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "interfaces/igamepad.h"

namespace io {
namespace factory {

class GamepadFactory {
public:
	explicit GamepadFactory() = delete;
	virtual ~GamepadFactory() = delete;

	static std::unique_ptr<io::interface::IGamepad> create(const common::enums::device type, std::string &&name, const u32 id);
	static bool configure(std::unique_ptr<io::interface::IGamepad> &interface);
};
}
}
