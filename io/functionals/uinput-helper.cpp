#include "uinput-helper.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace {
constexpr auto uinput_interface_paths = {
	"/dev/uinput",
	"/dev/input/uinput",
	"/dev/misc/uinput",
};
}

std::string UInputHelper::findUinputInterface() {
	for (auto path : uinput_interface_paths) {
		if (fs::is_character_file(path))
			return path;
	}

	return {};
}
