#pragma once

#include <list>
#include <memory>

#include "interfaces/iprofile.h"
#include "interfaces/igamepad.h"

namespace io {
namespace functional {

class Profile : public io::interface::IProfile {
public:
	using IProfile::IProfile;

private:
	std::list<std::unique_ptr<io::interface::IGamepad>> m_gamepads;
};
}
}
